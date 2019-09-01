#include <iostream>
#include "CPiece.hpp"
#ifndef CBoard_hpp
#define CBoard_hpp

///\file Declaration part of Cboard class.

///\brief Representing game board. The place there polymorphism is used.
class CBoard {
	public:
		CBoard() {}
		~CBoard() {
			for (int Row = 0; Row < 8; ++Row)
				for (int Col = 0; Col < 8; ++Col)
					//delete board[Row][Col]; deleting object? I like dat.
					board[Row][Col] = 0;
		}
		void InitBoard (std::vector<std::string> const PiecesPossitions);
		void Print (void);
		bool IsInCheck (const char color);
		bool IsPossibleMoves(const char color);
		CPiece* board[8][8]; ///< Matrix from pointers to pieces represented the game board.
};
#endif /* CBoard_hpp */

