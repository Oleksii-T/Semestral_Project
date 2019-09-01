#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

//'CPiece' - base abstract class for all exact 6 Peices.
		//'CPiece constructor' - initialize: color(W/B), stutus('used') of all Piecec to false(like was not used) and
		//fills vector 'PrevMove' by four zeros.
		//'GetPiece' - pure method returns char [P/N/R/B/Q/K] as a sign of exact Piece.
		//'GetColor' - returns color of Piece.
		//'GetPrevMoves' - returns coordinates used as a previous move.
		//'IsLegal' - pure method check whether move could be done by exact piece.
		//'coor' - actual color of Piece;
		//'PrevMove' - vector of coordinates used as a previus move.
		//'IsLegalMove' - checks is current move trying to take its own piece. If not call 'IsLegal', else return 'false'.
class CPiece {
	public:
		CPiece (char _color) : color(_color), PrevMove(std::vector<int>(4,0)) {};
		~CPiece() {}
		virtual char GetPiece() =0;
		char GetColor() {
			return color;
		}
		std::vector<int> GetPrevMoves(void) {
			return PrevMove;
		}
		bool IsLegalMove(int StartRow, int StartCol, int EndRow, int EndCol, CPiece* CurrBoard[8][8]) {
			CPiece* currTarget = CurrBoard[EndRow][EndCol];
			if ( currTarget == 0 || color != currTarget->GetColor() )
				return IsLegal(StartRow, StartCol, EndRow, EndCol, CurrBoard);
			return false;
		}
	private:
		virtual bool IsLegal(int StartRow, int StartCol, int EndRow, int EndCol, CPiece* CurrBoard[8][8]) =0;
	protected:
		char color;
		std::vector<int> PrevMove;
};


//=====================================================================================================================
//'CPawn' - inherited class from 'CPiece'. Responsible for Pawn.
class CPawn : public CPiece { // moves: ONE up
	public:
		CPawn(char color) : CPiece(color) {};
		~CPawn() {};
	private:
		virtual char GetPiece() {
			return 'P';
		}
		bool IsLegal(int StartRow, int StartCol, int EndRow, int EndCol, CPiece* CurrBoard[8][8]);
		bool DowblePawnMove(int StartRow, int StartCol, int EndRow, int EndCol);
		bool EnPassant(int StartRow, int StartCol, int EndRow, int EndCol, CPiece* CurrBoard[8][8]);
		bool CheckEnPassant (int StartRow, int EndRow, int StartEndCol, std::vector<int> trg);
		bool FillPrevMove(int StartRow, int StartCol, int EndRow, int EndCol);
};

//'Islegal'(Pawn) - check is such Pawn move is valid, determines special moves and call approptiate method
//	such as: 'DoublePawnMove', 'En-Passant'. When it find out that mvoe is valid, return 'FillPrevMove' method.
bool CPawn::IsLegal(int StartRow, int StartCol, int EndRow, int EndCol, CPiece* CurrBoard[8][8]) {
	CPiece* currTarget = CurrBoard[EndRow][EndCol];
	if (currTarget == 0) {
		// Make sure that way is empty
		if (StartCol == EndCol) {
			if (GetColor() == 'W') {
				if (EndRow == StartRow+1)
					return FillPrevMove(StartRow, StartCol, EndRow, EndCol);
				else
					return DowblePawnMove(StartRow, StartCol, EndRow, EndCol);
			}
			else
				if (EndRow == StartRow-1)
					return FillPrevMove(StartRow, StartCol, EndRow, EndCol);
				else
					return DowblePawnMove(StartRow, StartCol, EndRow, EndCol);
		}
		else
			return EnPassant(StartRow, StartCol, EndRow, EndCol, CurrBoard);
	}
	else {
		if (StartCol == EndCol+1 || StartCol == EndCol-1) {
			if (GetColor() == 'W') {
				if (EndRow == StartRow+1)
					return FillPrevMove(StartRow, StartCol, EndRow, EndCol);
			}
			else
				if (EndRow == StartRow-1)
					return FillPrevMove(StartRow, StartCol, EndRow, EndCol);
		}
	}
	return false;
}

//'DowblePawnMove' - method check the rest, and desides whether such move is valid, if yes returns
//	'FillPrevMove' methor othervise return false.
bool CPawn::DowblePawnMove(int StartRow, int StartCol, int EndRow, int EndCol) {
	if (GetColor() == 'W') {
		if ( EndRow == StartRow+2 && StartRow == 1)
			return FillPrevMove(StartRow, StartCol, EndRow, EndCol);
	}
	else
		if ( EndRow == StartRow-2 && StartRow == 6)
			return FillPrevMove(StartRow, StartCol, EndRow, EndCol);
	return false;
}

//'EnPassant' - method checks consistently the rest conditions foe successfull EnPassant, if averuthing is
//	OK, then take(dalete) enemy Pawn and return 'FillPrevMove' othervise, return false;
bool CPawn::EnPassant(int StartRow, int StartCol, int EndRow, int EndCol, CPiece* CurrBoard[8][8]) {
	int RO = (EndRow - StartRow > 0) ? 1 : -1; // shortcut for RowOffset
	int ColOffset = (EndCol - StartCol > 0) ? 1 : -1;
	//check columns
	if (EndCol == StartCol+(1*ColOffset)) 
		//check rows
		if (EndRow == StartRow+(1*RO)) 
			//check is there enemy Pawn on ajacent field
			if (CurrBoard[StartRow][StartCol+(1*RO)] != 0 ) 
				if (CurrBoard[StartRow][StartCol+(1*RO)]-> GetPiece() == 'P' && CurrBoard[StartRow][StartCol+(1*RO)]->GetColor() != color)
					//check previous move of enemy Pawn;
					if ( CheckEnPassant( EndRow+(1*RO), EndRow-(1*RO), EndCol, CurrBoard[EndRow-(1*RO)][EndCol]->GetPrevMoves() ) ) {
						//HERE ONE MORE CHECK SHOULD BE!!!!! (is dis move is done immedietly afer Enemy DowblePawnMove)
						CurrBoard[EndRow-(1*RO)][EndCol] = 0;
						return FillPrevMove(StartRow, StartCol, EndRow, EndCol);
					}
	return false;
}

//'CheckEnPassant' - used in 'EnPassant' method to be shure that previous move of appropriate enemy Pawn
//	meets the rules.
bool CPawn::CheckEnPassant (int StartRow, int EndRow, int StartEndCol, std::vector<int> trg) {
	if (trg[0] == StartRow && trg[1] == StartEndCol && trg[2] == EndRow && trg[3] == StartEndCol)
		return true;
	return false;
}

//'FillPrevMove' - Fills 'PrevMove' vector by current move, set 'used' to true and return true;
bool CPawn::FillPrevMove(int StartRow, int StartCol, int EndRow, int EndCol) {
	PrevMove[0] = StartRow;
	PrevMove[1] = StartCol;
	PrevMove[2] = EndRow;
	PrevMove[3] = EndCol;
	return true;	
}


//=====================================================================================================================
//'CKnight' - inherited class from 'CPiece'. Responsible for Knight.
class CKnight : public CPiece { // moves: 'L' like
	public:
		CKnight(char color) : CPiece(color) {};
		~CKnight() {};
	private:
		virtual char GetPiece() {
			return 'N';
		}
		bool IsLegal(int StartRow, int StartCol, int EndRow, int EndCol, CPiece* CurrBoard[8][8]);
};

bool CKnight::IsLegal(int StartRow, int StartCol, int EndRow, int EndCol, CPiece* CurrBoard[8][8]) {
	// Check that destination is whether empty or enemy field is already done in 'IsLegalMove'
	if (StartCol == EndCol+1 || StartCol == EndCol-1)
		if (StartRow == EndRow+2 || StartRow == EndRow-2)
			return true;
	if (StartCol == EndCol+2 || StartCol == EndCol-2)
		if (StartRow == EndRow+1 || StartRow == EndRow-1)
			return true;
	return false;
}


//=====================================================================================================================
//'CBishop' - inherited class from 'CPiece'. Responsible for Bishop.
class CBishop : public CPiece { // moves: DIAGINALS
	public:
		CBishop(char color) : CPiece(color) {};
		~CBishop() {};
	private:
		virtual char GetPiece() {
			return 'B';
		}
		bool IsLegal(int StartRow, int StartCol, int EndRow, int EndCol, CPiece* CurrBoard[8][8]);
};

bool CBishop::IsLegal(int StartRow, int StartCol, int EndRow, int EndCol, CPiece* CurrBoard[8][8]) {
	if (EndCol - StartCol == EndRow - StartRow || EndCol - StartCol == StartRow - EndRow) {
		int RowOffset = (EndRow - StartRow > 0) ? 1 : -1;
		int ColOffset = (EndCol - StartCol > 0) ? 1 : -1;
		// Make sure that way is empty
		for (int CheckRow=StartRow+RowOffset, CheckCol=StartCol+ColOffset ; CheckRow!=EndRow ; CheckRow+=RowOffset, CheckCol+=ColOffset)
			if (CurrBoard[CheckRow][CheckCol] != 0)
				return false;
		return true;
	}
	return false;
}


//=====================================================================================================================
//'CRook' - inherited class from 'CPiece'. Responsible for Rook.
class CRook : public CPiece { // moves: VERTICAL or HORIZONRAL
	public:
		CRook(char color) : CPiece(color) {};
		~CRook() {};
	private:
		virtual char GetPiece() {
			return 'R';
		}
		bool IsLegal(int StartRow, int StartCol, int EndRow, int EndCol, CPiece* CurrBoard[8][8]);
};

bool CRook::IsLegal(int StartRow, int StartCol, int EndRow, int EndCol, CPiece* CurrBoard[8][8]) {
	//Processing HORIZONRAL move
	if (StartRow == EndRow) {
		int ColOffset = (EndCol - StartCol > 0) ? 1 : -1;
		// Make sure that way is empty
		for (int CheckCol=StartCol+ColOffset ; CheckCol!=EndCol ; CheckCol+=ColOffset)
			if (CurrBoard[StartRow][CheckCol] != 0)
				return false;
		return true;
	}
	//Processing VERTICAL move
	else if (EndCol == StartCol) {
		int RowOffset = (EndRow - StartRow > 0) ? 1 : -1;
		// Make sure that way is empty
		for (int CheckRow=StartRow+RowOffset ; CheckRow!=EndRow ; CheckRow+=RowOffset)
			if (CurrBoard[CheckRow][StartCol] != 0)
				return false;
		return true;
	}
	return false;
}


//=====================================================================================================================
//'CQueen' - inherited class from 'CPiece'. Responsible for Queen.
class CQueen : public CPiece { // moves: VERTICAL or HORIZONRAL or DIAGONAL
	public:
		CQueen(char color) : CPiece(color) {};
		~CQueen() {};
	private:
		virtual char GetPiece() {
			return 'Q';
		}
		bool IsLegal(int StartRow, int StartCol, int EndRow, int EndCol, CPiece* CurrBoard[8][8]);
};

bool CQueen::IsLegal(int StartRow, int StartCol, int EndRow, int EndCol, CPiece* CurrBoard[8][8]) {
	//Compute additional variables;
	int ColOffset = (EndCol - StartCol > 0) ? 1 : -1;
	int RowOffset = (EndRow - StartRow > 0) ? 1 : -1;
	//Processing HORIZONRAL move
	if (StartRow == EndRow) {
		// Make sure that way is empty
		for (int CheckCol=StartCol+ColOffset ; CheckCol!=EndCol ; CheckCol+=ColOffset)
			if (CurrBoard[StartRow][CheckCol] != 0)
				return false;
		return true;
	}
	//Processing VERTICAL move
	else if (EndCol == StartCol) {
		// Make sure that way is empty
		for (int CheckRow=StartRow+RowOffset ; CheckRow!=EndRow ; CheckRow+=RowOffset)
			if (CurrBoard[CheckRow][StartCol] != 0)
				return false;
		return true;
	}
	//Processing DIAGONAL move
	else if (EndCol - StartCol == EndRow - StartRow || EndCol - StartCol == StartRow - EndRow) {
		// Make sure that way is empty
		for (int CheckRow=StartRow+RowOffset, CheckCol=StartCol+ColOffset ; CheckRow!=EndRow ; CheckRow+=RowOffset, CheckCol+=ColOffset)
			if (CurrBoard[CheckRow][CheckCol] != 0)
				return false;
		return true;
	}
	return false;
}


//=====================================================================================================================
//'CKing' - inherited class from 'CPiece'. Responsible for King.
class CKing : public CPiece { // moves: ONE adjacent field
	public:
		CKing(char color) : CPiece(color), IsCastlingDone(false) {};
		~CKing() {};
	private:
		virtual char GetPiece() {
			return 'K';
		}
		bool IsLegal(int StartRow, int StartCol, int EndRow, int EndCol, CPiece* CurrBoard[8][8]);
		bool Castling (int StartRow, int StartCol, int EndRow, int EndCol, CPiece* CurrBoard[8][8]);
		bool IsCastlingDone;
};

//'IsLegal' - Not only checks whether all trivial King move conditions are met, but determine Castlong and call
//	'Castling' method.
bool CKing::IsLegal(int StartRow, int StartCol, int EndRow, int EndCol, CPiece* CurrBoard[8][8]) {
	int RowSteps = EndRow - StartRow;
	int ColSteps = EndCol - StartCol;
	if (RowSteps >= -1 && RowSteps <= 1 && ColSteps >= -1 && ColSteps <= 1) {
		return true;
	}
	else if ( RowSteps == 0 && (ColSteps == -2 || ColSteps == 2))
		return Castling(StartRow, StartCol, EndRow, EndCol, CurrBoard);
	return false;
}

//'Castling' - Processes the castling. return [false/true] dependes on result.
bool CKing::Castling (int StartRow, int StartCol, int EndRow, int EndCol, CPiece* CurrBoard[8][8]) {
	int CO = (EndCol - StartCol > 0) ? 1 : -1; //Short cut for ColOffset
	//Instant check, to avoid Seg. fault further and was castling used already;
	if ( ((CO == 1 && EndCol+1 > 7) || (CO == -1 && EndCol-1 < 0)) && IsCastlingDone == false )
		return false;
	//check is ling on its start possition
	if ( (color=='W' && StartRow!=0 && StartCol!=3) || (color=='B' && StartRow!=7 && StartCol!=3) )
		return false;
	//On destination and adjacent field should be nothing;
	if (CurrBoard[EndRow][EndCol] == 0 && CurrBoard[StartRow][StartCol+(1*CO)] == 0) {
		//Is there something right after destination?;
		if ( CurrBoard[EndRow][EndCol+(1*CO)] != 0) {
			//Is it a COok?
			if (CurrBoard[EndRow][EndCol+(1*CO)]->GetPiece() == 'R' && CurrBoard[EndRow][EndCol+(1*CO)]->GetColor() == color) {
				if (EndCol == 0 || EndCol+(1*CO) == 7) //check is rook on its start pos  CurrBoard[EndRow][EndCol+(1*CO)]
					return false;
				CurrBoard[EndRow][EndCol-(1*CO)] = CurrBoard[EndRow][EndCol+(1*CO)];
				CurrBoard[EndRow][EndCol+(1*CO)] = 0;
				IsCastlingDone = true;
				return true;
			}
		}
		//may be COok is in one field after destination? Check is adjacent fiels is empty.
		else if ( CurrBoard[EndRow][EndCol+(1*CO)] == 0 ) {
			//Same as the vary first check but with an increased field of visibility.
			if ( (CO == 1 && EndCol+2 > 7) || (CO == -1 && EndCol-2 < 0) )
				return false;
			//here we know that there something after one empty field;
			if (CurrBoard[EndRow][EndCol+(2*CO)] != 0)
				//check is it true COok
				if (CurrBoard[EndRow][EndCol+(2*CO)]->GetPiece() == 'R' && CurrBoard[EndRow][EndCol+(2*CO)]->GetColor() == color) {
					if (EndCol == 0 || EndCol+(2*CO) == 7)//check is rook on its start pos  CurrBoard[EndRow][EndCol+(2*CO)]->IsUsed()]
						return false;
					CurrBoard[EndRow][EndCol-(1*CO)] = CurrBoard[EndRow][EndCol+(2*CO)];
					CurrBoard[EndRow][EndCol+(2*CO)] = 0;
					IsCastlingDone = true;
					return true;
				}
		}
	}
	return false;
}


//=====================================================================================================================
//'CBoard' - 
class CBoard {
	public:
		CBoard() {}
		~CBoard() {
			for (int Row = 0; Row < 8; ++Row)
				for (int Col = 0; Col < 8; ++Col)
					//delete board[Row][Col]; deleting object? I like dat.
					board[Row][Col] = 0;
		}
		void InitBoard (std::vector<std::string> PiecesPossitions);
		void Print (void);
		bool IsInCheck (char color);
		bool IsPossibleMoves(char color);
		CPiece* board[8][8];
};

void CBoard::InitBoard (std::vector<std::string> PiecesPossitions) {
	std::stringstream ss;
	std::string CoordinatesString;
	char Color, Piece;
	int Coordinates, Row, Col;
	for (Row = 0; Row < 8; ++Row)
		for (Col = 0; Col < 8; ++Col)
			board[Row][Col] = 0;
	for (auto i :  PiecesPossitions) {
		CoordinatesString = i.substr(0,2);
		ss << CoordinatesString;
		ss >> Coordinates;
		Row = (Coordinates / 10) - 1;
		Col = (Coordinates % 10) - 1;
		Color = i.substr(2,2)[0];
		Piece = i.substr(3,3)[0];
		switch (Piece) {
			case 'N': board[Row][Col] = new CKnight(Color); break;
			case 'R': board[Row][Col] = new CRook(Color); break;
			case 'B': board[Row][Col] = new CBishop(Color); break;
			case 'Q': board[Row][Col] = new CQueen(Color); break;
			case 'K': board[Row][Col] = new CKing(Color); break;
			case 'P': board[Row][Col] = new CPawn(Color); break;
		}
		ss.str("");
		ss.clear();
	}
}

void CBoard::Print (void) {
	int BlockHeight = 3; // amount of characters user to build each field in Height
	int BlockWidth = 4; // amount of characters user to build each field in Width
	//drowing left side
	for (int row=0 ; row < 8*BlockHeight ; ++row) {
		int iSquareRow = row/BlockHeight;
		if (row % 3 == 1)
		 std::cout << "-" << 8-iSquareRow << "-";
		else
			std::cout << "---";
		//drowing board
		for (int col=0 ; col < 8*BlockWidth ; ++col) {
			int iSquareCol = col/BlockWidth;
			if (((row % 3) == 1) && ((col % 4) == 1 || (col % 4) == 2) && board[7-iSquareRow][iSquareCol] != 0) {
				if ((col % 4) == 1)
					std::cout << board[7-iSquareRow][iSquareCol]->GetColor();
					else
					std::cout << board[7-iSquareRow][iSquareCol]->GetPiece();
			} 
			else {
				if ((iSquareRow + iSquareCol) % 2 == 1)
					std::cout << '*';
				else
					std::cout << ' ';
			}
		}
	std::cout << std::endl;
	}
	//drowing bottom
	for (int row = 0; row < BlockHeight; ++row) {
		if (row % 3 == 1) {
			std::cout << "---";
			for (int col = 0; col < 8*BlockWidth; ++col) {
				if ( col % 4 == 1 )
					std::cout << col/BlockWidth+1;
				else
					std::cout << "-";
			}
		}
		else 
			for (int row = 1 ; row < 9*BlockWidth ; ++row)
				std::cout << "-";
		std::cout << std::endl;
	}
}

bool CBoard::IsInCheck (char color) {
	int KingRow=-1;
	int KingCol=-1;
	//find king;
	for (int StartRow = 0; StartRow < 8; ++StartRow)
		for (int StartCol = 0; StartCol < 8; ++StartCol) 
			if (board[StartRow][StartCol] != 0)
				if (board[StartRow][StartCol]->GetColor() == color)
					if (board[StartRow][StartCol]->GetPiece() == 'K') {
						KingRow = StartRow;
						KingCol = StartCol;
					}
			
	for (int StartRow = 0; StartRow < 8; ++StartRow)
		for (int StartCol = 0; StartCol < 8; ++StartCol)
			if (board[StartRow][StartCol] != 0)
				if (board[StartRow][StartCol]->GetColor() != color)
					if (board[StartRow][StartCol]->IsLegalMove(StartRow, StartCol, KingRow, KingCol, board))
						return true;
	return false;
}

bool CBoard::IsPossibleMoves(char color) {
	// Run through all pieces
	for (int StartRow = 0; StartRow < 8; ++StartRow) 
		for (int StartCol = 0; StartCol < 8; ++StartCol) 
			if (board[StartRow][StartCol] != 0) //check is it a Piece at all
				// If it is a piece of the current player, see if it has a legal move
				if (board[StartRow][StartCol]->GetColor() == color) 
					for (int EndRow = 0; EndRow < 8; ++EndRow) 
						for (int EndCol = 0;EndCol < 8; ++EndCol) 
							if (board[StartRow][StartCol]->IsLegalMove(StartRow, StartCol, EndRow, EndCol, board)) {
								// Make move and check whether king is in check
								CPiece* tmp	= board[EndRow][EndCol];
								board[EndRow][EndCol]	= board[StartRow][StartCol];
								board[StartRow][StartCol]	= 0;
								bool FullValidMove = !IsInCheck(color);
								// Undo the move
								board[StartRow][StartCol] = board[EndRow][EndCol];
								board[EndRow][EndCol]	= tmp;
								if (FullValidMove) {
									return true;
								}
							}
	return false;
}

//=====================================================================================================================
class CChess {
	public:
		CChess(char* _SafeFile) : SafeFile(_SafeFile), Turn('W') {}
		~CChess() {}
		void Start();
		bool FileIsValid(char* SafeFile);
		void FillDefPositions (std::vector<std::string>& PiecesPossitions);
		bool ReadFileToVector (char* SafeFile, std::vector<std::string>& PiecesPossitions);
		void GetNextMove(CPiece* board[8][8]);
		int ScanMove (int& Row, int& Col, CPiece* CurrBoard[8][8]);
		bool IsDigit(const std::string &str) {
	    return str.find_first_not_of("0123456789") == std::string::npos; }
		void Save(CPiece* CurrBoard[8][8]);
		inline bool Exist (const std::string& name);
		void PawnPromotion(int StartRow, int StartCol, int EndRow, int EndCol, CPiece* CurrBoard[8][8]);
		void PawnPromotionHelper(int StartRow, int StartCol, CPiece* CurrBoard[8][8], char color);
		void SwitchTurn() {
			Turn = (Turn == 'W') ? 'B' : 'W'; }
		bool GameIsOver(void);
	private:
		char* SafeFile;
		CBoard GameBoard;
		char Turn;
};

void CChess::Start() {
	std::vector<std::string> PiecesPossitions;
	if (SafeFile == NULL)
		FillDefPositions(PiecesPossitions);
	else
		if ( !Exist(SafeFile) || !FileIsValid(SafeFile) || !ReadFileToVector(SafeFile, PiecesPossitions)) {
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

bool CChess::FileIsValid(char* SafeFile) {
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

bool CChess::ReadFileToVector (char* SafeFile, std::vector<std::string>& PiecesPossitions) {
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

void CChess::Save(CPiece* CurrBoard[8][8]) {		
	std::string name1 = "GameSaves/Game_", name;
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

inline bool CChess::Exist (const std::string& name) {
	 if (FILE *file = fopen(name.c_str(), "r")) {
		    fclose(file);
		    return true;
	 } else {
		    return false;
		}
}

void CChess::PawnPromotion(int StartRow, int StartCol, int EndRow, int EndCol, CPiece* CurrBoard[8][8]) {
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

void CChess::PawnPromotionHelper(int StartRow, int StartCol, CPiece* CurrBoard[8][8], char color) {
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
			default: IsValid=false; break; //if Piece doesn seems like correct one, set IsValid to false
		}
	} while (!IsValid); // continue loop while valid piece wount bo choosen;
}

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


int main (int argc, char* argv[]) {
	CChess chess(argv[1]);
	chess.Start();
return 0;
}



//echo "[2009/05/11 07:30:00]" -> 2009_05_..._00 value | sort -k1,1
