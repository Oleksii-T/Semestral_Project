#ifndef CPiece_hpp
#define CPiece_hpp
#include <iostream>
#include <vector>

///\file Declaration part of CPiece class.

/// Base abstract class for all exact 6 Pieces.
class CPiece {
	public:
		CPiece (char _color) : color(_color), PrevMove(std::vector<int>(4,0)) {};
		~CPiece() {}
		virtual char GetPiece() =0;
		char GetColor() {
			return color; }
		std::vector<int> GetPrevMoves(void) {
			return PrevMove; }
		bool IsLegalMove(const int StartRow, const int StartCol, const int EndRow, const int EndCol, CPiece* CurrBoard[8][8]);
	private:
		virtual bool IsLegal(const int StartRow, const int StartCol, const int EndRow, const int EndCol, CPiece* CurrBoard[8][8]) =0;
	protected:
		char color; ///< Color of the exact piece.
		std::vector<int> PrevMove; ///< Vector filled by previous move of piece. Used to check Castling and EnPassant.
};


/// Inherited class from 'CPiece'. Responsible for Pawn.
class CPawn : public CPiece { // moves: ONE up
	public:
		CPawn(char color) : CPiece(color) {};
		~CPawn() {};
	private:
		virtual char GetPiece() {
			return 'P';
		}
		bool IsLegal(const int StartRow, const int StartCol, const int EndRow, const int EndCol, CPiece* CurrBoard[8][8]);
		bool DowblePawnMove(const int StartRow, const int StartCol, const int EndRow, const int EndCol);
		bool EnPassant(const int StartRow, const int StartCol, const int EndRow, const int EndCol, CPiece* CurrBoard[8][8]);
		bool CheckEnPassant (const int StartRow, const int EndRow, const int StartEndCol, const std::vector<int> trg);
		bool FillPrevMove(const int StartRow, const int StartCol, const int EndRow, const int EndCol);
};


/// Inherited class from 'CPiece'. Responsible for Knight.
class CKnight : public CPiece { // moves: 'L' like
	public:
		CKnight(char color) : CPiece(color) {};
		~CKnight() {};
	private:
		virtual char GetPiece() {
			return 'N';
		}
		bool IsLegal(const int StartRow, const int StartCol, const int EndRow, const int EndCol, CPiece* CurrBoard[8][8]);
};

/// Inherited class from 'CPiece'. Responsible for Bishop.
class CBishop : public CPiece { // moves: DIAGINALS
	public:
		CBishop(char color) : CPiece(color) {};
		~CBishop() {};
	private:
		virtual char GetPiece() {
			return 'B';
		}
		bool IsLegal(const int StartRow, const int StartCol, const int EndRow, const int EndCol, CPiece* CurrBoard[8][8]);
};

/// Inherited class from 'CPiece'. Responsible for Rook.
class CRook : public CPiece { // moves: VERTICAL or HORIZONRAL
	public:
		CRook(char color) : CPiece(color) {};
		~CRook() {};
	private:
		virtual char GetPiece() {
			return 'R';
		}
		bool IsLegal(const int StartRow, const int StartCol, const int EndRow, const int EndCol, CPiece* CurrBoard[8][8]);
};

/// Inherited class from 'CPiece'. Responsible for Queen.
class CQueen : public CPiece { // moves: VERTICAL or HORIZONRAL or DIAGONAL
	public:
		CQueen(char color) : CPiece(color) {};
		~CQueen() {};
	private:
		virtual char GetPiece() {
			return 'Q';
		}
		bool IsLegal(const int StartRow, const int StartCol, const int EndRow, const int EndCol, CPiece* CurrBoard[8][8]);
};

/// Inherited class from 'CPiece'. Responsible for King.
class CKing : public CPiece { // moves: ONE adjacent field
	public:
		CKing(char color) : CPiece(color), IsCastlingDone(false) {};
		~CKing() {};
	private:
		virtual char GetPiece() {
			return 'K';
		}
		bool IsLegal(const int StartRow, const int StartCol, const int EndRow, const int EndCol, CPiece* CurrBoard[8][8]);
		bool Castling (const int StartRow, const int StartCol, const int EndRow, const int EndCol, CPiece* CurrBoard[8][8]);
		bool IsCastlingDone; ///< Variable, thanks to which we have an opportuniti to check whether Castling was already done.
};
#endif /* CPiece_hpp */
