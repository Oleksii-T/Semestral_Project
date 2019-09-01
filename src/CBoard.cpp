#include <iostream>
#include "CPiece.hpp"
#include "CBoard.hpp"
#include <vector>
#include <string>
#include <sstream>

///\file Implementation part of CBoard class.

/*!
Initialization of game board.
\param PiecesPossitions All setting of pieces(what and where should be placed) is stored in this vector.
*/
void CBoard::InitBoard (const std::vector<std::string> PiecesPossitions) {
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


///\details Game board printing.
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

/*!
Checks whether King of current player is in check after move which he/she done.
Firstly it finds King, and then for each Piece of enemy check whether it could take the King.
\param color Color of current player.
\return 'true'-Check, 'false'-safe.
*/
bool CBoard::IsInCheck (const char color) {
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

/*!
Check is there possible moves for player, otherwise, he/she lost.
So, it finds all pieces then for each finds all possible(moreover valid) ways to move,
then checks whether there any such move when King is still in safe.
\return 'true'-There safe move, 'false'-no valid move.
\param color Color of current player. 
*/
bool CBoard::IsPossibleMoves(const char color) {
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
