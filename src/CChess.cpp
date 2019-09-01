#include <iostream>
#include "CChess.hpp"
#include "CPiece.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

///\file Implementation part of CChess class.

/*!
Method which start the game.
Processing load file using such methods:
 'FillDefPositions', 'Exist', 'FileIsValid' and 'ReadFileToVector'.
Then initialization of game board with 'InitBoard'(CBoard) method.
Then the main game loop take place. It includes sequential calls of such methods:
 'GetNextMove', 'SwitchTurn' and 'GameIsOver'.
And in the very end of the game, printing of current game board with 'Print'(CBoard).
*/
void CChess::Start() {
	std::vector<std::string> PiecesPossitions;
	if (SafeFile == NULL)
		FillDefPositions(PiecesPossitions);
	else
		if ( !Exist((std::string)SafeFile) || !FileIsValid(SafeFile) || !ReadFileToVector(SafeFile, PiecesPossitions)) {
			std::cout << "SafeFile '" << SafeFile <<  "' is invalid. Drowing default board..." << std::endl;
			FillDefPositions(PiecesPossitions);
		}
	GameBoard.InitBoard(PiecesPossitions);
	do {
		GetNextMove(GameBoard.board);
		SwitchTurn();
	} while (!GameIsOver());
	GameBoard.Print();
}

/*!
Method is aimed at checking the file whether is valid or no.
\param SafeFile Name of load file passed by used.
\return bool means whether load file is valid(true if yes).
*/
bool CChess::FileIsValid(const char* SafeFile) {
	std::ifstream  SafeFileStream;
	std::string line;
	SafeFileStream.open(SafeFile);
	bool IsValid;
	int NumberOfWK=0, NumberOfBK=0;
	while (std::getline(SafeFileStream, line)) {
		IsValid = false;
		if (line.size() == 4) //size must be 4
			if (isdigit(line[0]) && line[0] != 9 && line[0] != 0) //0 element must be digit within [1,8]
				if (isdigit(line[1]) && line[1] != 9 && line[1] != 0) //1 element must be digit within [1,8]
					if (isalpha(line[2]) && isalpha(line[3])) //2 and 3 element must be letter
						if  ( line[2]=='W' || line[2]=='B' ) //2 element must be as a valid name of Piece
							//3 element must be as a valid name of Piece 
							if  ( line[3]=='P' || line[3]=='N' || line[3]=='R' || line[3]=='B' || line[3]=='Q' || line[3]=='K' ) {
								IsValid = true;
								if (line[2] == 'W' && line[3] == 'K')
									NumberOfWK += 1;
								if (line[2] == 'B' && line[3] == 'K')
									NumberOfBK += 1;
									}
		if (!IsValid)
			return false;
	}
	if (NumberOfWK != 1 || NumberOfBK != 1)
		return false;
	SafeFileStream.close();
	return true;
}

/*!
If program determines that user did not specify any load file it will fill
'PiecesPossitions' by itself with default values according to the chess rules
\param PiecesPossitions All setting of pieces(what and where should be placed) is stored in this vector.
*/
void CChess::FillDefPositions (std::vector<std::string>& PiecesPossitions) {
	PiecesPossitions.clear();
	std::stringstream ss;
	//Init While Pieces
	std::vector<char> turns = {'W','B'};
	int TargetLine;
	//Init All Pieces exept Pawns;
	for (auto i:turns) {
		TargetLine = (i=='W') ? 1 : 8;
		for (int j=1;j<9;++j) {
				if (j==1 || j==8) {
					ss << TargetLine << j << i << "R";
					PiecesPossitions.push_back(ss.str());
					ss.str(""); }
				else if (j==2 || j==7) {
					ss << TargetLine << j << i << "N";
					PiecesPossitions.push_back(ss.str());
					ss.str(""); }
				else if (j==3 || j==6) {
					ss << TargetLine << j << i << "B";
					PiecesPossitions.push_back(ss.str());
					ss.str(""); }
				else if (j==4) {
					ss << TargetLine << j << i << "K";
					PiecesPossitions.push_back(ss.str());
					ss.str(""); }
				else if (j==5) {
					ss << TargetLine << j << i << "Q";
					PiecesPossitions.push_back(ss.str());
					ss.str(""); }
		}
	}
	//Init While Pawns
		for (int i=1;i<9;++i) {
		ss << "2" << i << "W" << "P";
		PiecesPossitions.push_back(ss.str());
		ss.str("");
	}
	//Init Black Pawns
	for (int i=1;i<9;++i) {
		ss << "7" << i << "B" << "P";
		PiecesPossitions.push_back(ss.str());
		ss.str("");
	}
}

/*!
If file is valid this method is called.
It converts load file into more convenient form, to facilitate the further processing.
\param SafeFile Name of load file passed by used.
\param PiecesPossitions All setting of pieces(what and where should be placed) is stored in this vector.
\return bool means whether file was successfully converted or no.
*/
bool CChess::ReadFileToVector (const char* SafeFile, std::vector<std::string>& PiecesPossitions) {
	PiecesPossitions.clear();
	std::ifstream SafeFileStream;
	SafeFileStream.open(SafeFile);
	if (SafeFileStream.fail())
		return false;
	std::string line;
	while (std::getline(SafeFileStream, line))
		PiecesPossitions.push_back(line);
			SafeFileStream.close();
			return true;
}

/*!
Scan move of next player. This is the place there program does the biggest part of work.
\param board Currently valid game board. Further could be used as 'CurrBoard'.
*/
void CChess::GetNextMove(CPiece* board[8][8]) {
	//flag shows was move valid or no. if no, loop take place.
	bool IsValid=false; 
	int StartRow, StartCol, EndRow, EndCol;
	do {
		GameBoard.Print();
		std::cout << Turn <<  "`s moves: ";
		int ScanMoveStart (ScanMove(StartRow, StartCol, board));
		if (ScanMoveStart == -1)
			continue;
		std::cout << "       To: ";
		int ScanMoveEnd (ScanMove(EndRow, EndCol, board));
		if (ScanMoveEnd == -1)
			continue;

		if (!ScanMoveStart || !ScanMoveEnd) {
				std::cout << "Invalid move!" << std::endl;
				continue;
		}
		//check coordinates.
		if ( StartRow<=7 && StartRow>=0 && StartCol<=7 && StartCol>=0 &&
				 EndRow<=7   && EndRow>=0   && EndCol<=7   && EndCol>=0 ) {
			CPiece* currP = board[StartRow][StartCol];
			//check is piese is choosen correctly.
			if (currP != 0 && currP->GetColor() == Turn)
				//is dat valid move?
				if ( currP->IsLegalMove(StartRow, StartCol, EndRow, EndCol, board) ) {
					//all is Ok, lets make a move.
					PawnPromotion(StartRow, StartCol, EndRow, EndCol, board);
					CPiece* tmp	= board[EndRow][EndCol]; //remember Poece which is taken;
					board[EndRow][EndCol] = board[StartRow][StartCol];
					board[StartRow][StartCol] = 0;
					//need to make additional check here(is piece is in check).
					IsValid=true;
					if (GameBoard.IsInCheck(Turn)) {
						IsValid=false;
						//undo last move
						board[StartRow][StartCol] = board[EndRow][EndCol];
						board[EndRow][EndCol] = tmp;
					}
				}
		}
		if (!IsValid)
			std::cout << "Invalid move!" << std::endl;
	} while (!IsValid);
}

/*!
Scanning new move and immediate check it format.
Saves the game by calling 'Save' if used wanted so.
\param Row,Col Actual coordinates, which is scanned.
\param CurrBoard Currently valid game board.
\return 3 cases: '1'-Success, '0'-Fail, '-1'-Game was saved.
*/
int CChess::ScanMove (int& Row, int& Col, CPiece* CurrBoard[8][8]) {
	std::string RowString, ColString;
	std::stringstream ss;
	std::string Move;
			
	std::cin >> Move;
			
	if (Move == "safe") {
		Save(CurrBoard);
		return -1;
	}
			
	if (Move.size() == 2)
		if (IsDigit(Move)) {
			RowString = Move[0];
			ColString = Move[1];
			ss << RowString;
			ss >> Row;
			ss.str("");
			ss.clear();
			ss << ColString;
			ss >> Col;
			Row -= 1;
			Col -= 1;
					
			return 1;
		}
	return 0;
}

/*!
Saving board into valid file.
\param CurrBoard Currently valid game board.
*/
void CChess::Save(CPiece* CurrBoard[8][8]) {		
	std::string name1 = "Game_", name;
	std::ofstream SaveFile;
	int i;
	for (i=1;;++i) {
		name = name1 + std::to_string(i) + ".txt";
		if (!Exist(name))
			break;
	}
	std::cout << "Game saved in: '" << name << "'." << std::endl; 
	SaveFile.open(name);
	char Color, Piece;
	for (int row=0 ; row < 8 ; ++row)
		for (int col=0 ; col < 8 ; ++col)
			if (CurrBoard[row][col] != 0) {
				Color = CurrBoard[row][col]->GetColor();
				Piece = CurrBoard[row][col]->GetPiece();
				SaveFile << row+1 << col+1 << Color << Piece << "\n";
			}
	SaveFile.close();
}

/*!
Check whether file is exist.
\param name Name of load file to be checked.
\return 'true'-Exist, 'false'-Not.
*/
inline bool CChess::Exist (const std::string name) {
	 if (FILE *file = fopen(name.c_str(), "r")) {
		    fclose(file);
		    return true;
	 } else {
		    return false;
		}
}

/*!
With the help of 'PawnPromotionHelper' process Dowble Pawn Move.
On this stage we just check correctness of move, if it is fine, call helper method.
\param StartRow,StartCol Start coordinates.
\param EndRow,EndCol Destination coordinates.
\param CurrBoard Currently valid game board.
*/
void CChess::PawnPromotion(const int StartRow, const int StartCol, const int EndRow, const int EndCol, CPiece* CurrBoard[8][8]) {
	char color = CurrBoard[StartRow][StartCol]->GetColor();
	if (CurrBoard[StartRow][StartCol]->GetPiece() == 'P') {
		if (color == 'W') {
			if (EndRow == 7)
				PawnPromotionHelper(StartRow, StartCol, CurrBoard, color);
		}
		else
			if (EndRow == 0)
				PawnPromotionHelper(StartRow, StartCol, CurrBoard, color);
	}
}

/*!
Actual Pawn substitution on other piece.
\param StartRow,StartCol Current coordinates of Pawn.
\param CurrBoard Currently valid game board.
*/
void CChess::PawnPromotionHelper(const int StartRow, const int StartCol, CPiece* CurrBoard[8][8], char color) {
	char NewPiece;
	std::string NewPieceString;
	bool IsValid;
	std::cout << "Enter name of Piece(N/R/B/Q) on which you want to replace your Pawn: ";
	do {
		std::cin >> NewPieceString;
		if (NewPieceString.size() == 1) //must consist only one char;
			NewPiece = NewPieceString[0];
		else {                          //print start scaning again
			std::cout << "Invalid Piece, enter again: ";
			continue;
		}
		CurrBoard[StartRow][StartCol] = 0;//erase Pawn;
		switch (NewPiece) { //and place on this field new Piece, choosen by player
			case 'N': CurrBoard[StartRow][StartCol] = new CKnight(color); IsValid=true; break; // If it is valid,
			case 'R': CurrBoard[StartRow][StartCol] = new CRook(color); IsValid=true; break;   // piece, initialize 
			case 'B': CurrBoard[StartRow][StartCol] = new CBishop(color); IsValid=true; break; // it and set var
			case 'Q': CurrBoard[StartRow][StartCol] = new CQueen(color); IsValid=true; break;  // IsValid to true
			default: IsValid=false; break; //if Piece doesnt seems like correct one, set IsValid to false
		}
	} while (!IsValid); // continue loop while valid piece won't be chosen;
}

/*!
Check is there any possible move for player.
\return 'true'-game is over, 'false'-game can be continued. 
*/
bool CChess::GameIsOver(void) {
	// Check that the current player can move
	// If not, we have a stalemate or checkmate
	bool CanMove = GameBoard.IsPossibleMoves(Turn); //true if there possible moves, false if end
	if (!CanMove) {
		if (GameBoard.IsInCheck(Turn)) {
			SwitchTurn();
			std::cout << "Checkmate, " << Turn << " Wins!" << std::endl;
		}
		else
			std::cout << "Stalemate!" << std::endl;
	}
	return !CanMove;
}
