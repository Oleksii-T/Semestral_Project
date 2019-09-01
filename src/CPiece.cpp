#include <iostream>
#include <vector>
#include "CPiece.hpp"

///\file Implementation part of CPiece class.

/*!
Method called in 'CChess' regardless which piece is proccess now.
Just check did used move on his own piece.
Then calls appropriate(for exact piece) check method.
\param StartRow,StartCol Coordinates from which player wants to move.
\param EndRow,EndCol Coordinates to which player wants to move.
\param CurrBoard Currently valid game board.
\return 'true'-move is valid and 'false' if not.
*/
bool CPiece::IsLegalMove(const int StartRow, const int StartCol, const int EndRow, const int EndCol, CPiece* CurrBoard[8][8]) {
	CPiece* currTarget = CurrBoard[EndRow][EndCol];
	if ( currTarget == 0 || color != currTarget->GetColor() )
		return IsLegal(StartRow, StartCol, EndRow, EndCol, CurrBoard);
	return false;
}


/*!
Checks is such Pawn move is valid, determines special moves and call approptiate methods
such as: 'DoublePawnMove', 'En-Passant'.
When it find out that mvoe is valid, return 'FillPrevMove' method.
\param StartRow,StartCol Coordinates from which player wants to move.
\param EndRow,EndCol Coordinates to which player wants to move.
\param CurrBoard Currently valid game board.
\return 'true'-pawn move is valid and 'false' if not.
*/
bool CPawn::IsLegal(const int StartRow, const int StartCol, const int EndRow, const int EndCol, CPiece* CurrBoard[8][8]) {
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

/*!
Check the correctness of double pawn move, if it is valid calls 'FillPrevMove' method.
\param StartRow,StartCol Coordinates from which player wants to move.
\param EndRow,EndCol Coordinates to which player wants to move.
\return 'true'-double pawn move is valid and 'false' if not.
*/
bool CPawn::DowblePawnMove(const int StartRow, const int StartCol, const int EndRow, const int EndCol) {
	if (GetColor() == 'W') {
		if ( EndRow == StartRow+2 && StartRow == 1)
			return FillPrevMove(StartRow, StartCol, EndRow, EndCol);
	}
	else
		if ( EndRow == StartRow-2 && StartRow == 6)
			return FillPrevMove(StartRow, StartCol, EndRow, EndCol);
	return false;
}

/*!
Checks conditions for successful EnPassant, if everything is OK,
then take(delete) enemy Pawn and call 'FillPrevMove'.
\param StartRow,StartCol Coordinates from which player wants to move.
\param EndRow,EndCol Coordinates to which player wants to move.
\param CurrBoard Currently valid game board.
\return 'true'-EnPassant is valid and 'false' if not.
*/
bool CPawn::EnPassant(const int StartRow, const int StartCol, const int EndRow, const int EndCol, CPiece* CurrBoard[8][8]) {
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
						//HERE ONE MORE CHECK SHOULD BE!!!!! (is dis move is done immedietly afer Enemy DoublePawnMove)
						CurrBoard[EndRow-(1*RO)][EndCol] = 0;
						return FillPrevMove(StartRow, StartCol, EndRow, EndCol);
					}
	return false;
}

/*!
Used in 'EnPassant' method to be sure that previous move of appropriate enemy Pawn meets the rules.
\param StartRow,StartEndCol Coordinates from which player wants to move.
\param EndRow,StartEndCol Coordinates to which player wants to move.
\param trg Vector of previous move of appropriate enemy pawn.
\return 'true'-Previous move was double pawn move and 'false' if not.
*/
bool CPawn::CheckEnPassant (const int StartRow, const int EndRow, const int StartEndCol, const std::vector<int> trg) {
	if (trg[0] == StartRow && trg[1] == StartEndCol && trg[2] == EndRow && trg[3] == StartEndCol)
		return true;
	return false;
}

/*
Fills 'PrevMove' vector by current move, set 'used' to true and return true;
\param StartRow,StartCol Coordinates from which player wants to move.
\param EndRow,EndCol Coordinates to which player wants to move.
\return 'true'-always, to save one line(not to call whis method and then return true, but immediately return method).
*/
bool CPawn::FillPrevMove(const int StartRow, const int StartCol, const int EndRow, const int EndCol) {
	PrevMove[0] = StartRow;
	PrevMove[1] = StartCol;
	PrevMove[2] = EndRow;
	PrevMove[3] = EndCol;
	return true;	
}


/*!
Checks whether such Knight move is valid.
\param StartRow,StartCol Coordinates from which player wants to move.
\param EndRow,EndCol Coordinates to which player wants to move.
\param CurrBoard Currently valid game board.
\return 'true'-move is valid and 'false' if not.
*/
bool CKnight::IsLegal(const int StartRow, const int StartCol, const int EndRow, const int EndCol, CPiece* CurrBoard[8][8]) {
	// Check that destination is whether empty or enemy field is already done in 'IsLegalMove'
	if (StartCol == EndCol+1 || StartCol == EndCol-1)
		if (StartRow == EndRow+2 || StartRow == EndRow-2)
			return true;
	if (StartCol == EndCol+2 || StartCol == EndCol-2)
		if (StartRow == EndRow+1 || StartRow == EndRow-1)
			return true;
	return false;
}

/*!
Checks whether such Bishop move is valid.
\param StartRow,StartCol Coordinates from which player wants to move.
\param EndRow,EndCol Coordinates to which player wants to move.
\param CurrBoard Currently valid game board.
\return 'true'-move is valid and 'false' if not.
*/
bool CBishop::IsLegal(const int StartRow, const int StartCol, const int EndRow, const int EndCol, CPiece* CurrBoard[8][8]) {
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


/*!
Checks whether such Rook move is valid.
\param StartRow,StartCol Coordinates from which player wants to move.
\param EndRow,EndCol Coordinates to which player wants to move.
\param CurrBoard Currently valid game board.
\return 'true'-move is valid and 'false' if not.
*/
bool CRook::IsLegal(const int StartRow, const int StartCol, const int EndRow, const int EndCol, CPiece* CurrBoard[8][8]) {
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


/*!
Checks whether such CQueen move is valid.
\param StartRow,StartCol Coordinates from which player wants to move.
\param EndRow,EndCol Coordinates to which player wants to move.
\param CurrBoard Currently valid game board.
\return 'true'-move is valid and 'false' if not.
*/
bool CQueen::IsLegal(const int StartRow, const int StartCol, const int EndRow, const int EndCol, CPiece* CurrBoard[8][8]) {
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


/*!
Checks whether such King move is valid, determines Castling, then call additional methods to process it.
\param StartRow,StartCol Coordinates from which player wants to move.
\param EndRow,EndCol Coordinates to which player wants to move.
\param CurrBoard Currently valid game board.
\return 'true'-move is valid and 'false' if not.
*/
bool CKing::IsLegal(const int StartRow, const int StartCol, const int EndRow, const int EndCol, CPiece* CurrBoard[8][8]) {
	int RowSteps = EndRow - StartRow;
	int ColSteps = EndCol - StartCol;
	if (RowSteps >= -1 && RowSteps <= 1 && ColSteps >= -1 && ColSteps <= 1) {
		return true;
	}
	else if ( RowSteps == 0 && (ColSteps == -2 || ColSteps == 2))
		return Castling(StartRow, StartCol, EndRow, EndCol, CurrBoard);
	return false;
}

/*!
Processes the castling.
\param StartRow,StartCol Coordinates from which player wants to move.
\param EndRow,EndCol Coordinates to which player wants to move.
\param CurrBoard Currently valid game board.
\return 'true'-pawn move is valid and 'false' if not.
*/
bool CKing::Castling (const int StartRow, const int StartCol, const int EndRow, const int EndCol, CPiece* CurrBoard[8][8]) {
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
		//may be Rook is in one field after destination? Check is adjacent fiels is empty.
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
