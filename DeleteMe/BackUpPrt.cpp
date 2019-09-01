#include <iostream>
#include <vector>
#include <memory>

//'IsLegalMove' - checks is current for trying to take your own piece. If not call 'IsLegal', else return 'false'.
//'IsLegal' - check can move could be done by exact piece.
class CPiece {
	public:
		CPiece (char _color) : color(_color), used(false), PrevMove(std::vector<int>(4,0)) {};
		~CPiece() {}
		virtual char GetPiece() =0;
		virtual bool IsUsed() =0;
		char GetColor() {
			return color;
		}
		std::vector<int> GetPrevMoves(void) {
			return PrevMove;
		}
		bool IsLegalMove(int StartRow, int StartCol, int EndRow, int EndCol, std::unique_ptr<CPiece> CurrBoard[8][8]) {
			std::unique_ptr<CPiece> currTarget = std::move(CurrBoard[EndRow][EndCol]);
			if ( currTarget == 0 || color != currTarget->GetColor() ) {
				return IsLegal(StartRow, StartCol, EndRow, EndCol, CurrBoard);
			}
			return false;
		}
	private:
		virtual bool IsLegal(int StartRow, int StartCol, int EndRow, int EndCol, std::unique_ptr<CPiece> CurrBoard[8][8]) =0;
	protected:
		char color;//it is protected cause we will use it in 'IsLegal' method in 'CKing' claass.
		bool used;
		std::vector<int> PrevMove;//small array store previous move of Pawn.(used for en-passant)
};


//==================================================================================================================================================
class CPawn : public CPiece { // moves: ONE up
	public:
		CPawn(char color) : CPiece(color) {};
		~CPawn() {};
	private:
		virtual bool IsUsed() {
			return used;
		}
		virtual char GetPiece() {
			return 'P';
		}
		bool IsLegal(int StartRow, int StartCol, int EndRow, int EndCol, std::unique_ptr<CPiece> CurrBoard[8][8]) {
			std::unique_ptr<CPiece> currTarget = std::move(CurrBoard[EndRow][EndCol]);
			if (currTarget == 0) {
				// Make sure that way is empty
				if (StartCol == EndCol) {
					if (GetColor() == 'W') {
						if (EndRow == StartRow+1) {
							if (EndRow == 7)
								return PawnPromotion (StartRow, StartCol, CurrBoard);
							return FillPrevMove(StartRow, StartCol, EndRow, EndCol);
						}
						else
							return DowblePawnMove(StartRow, StartCol, EndRow, EndCol);
					}
					else
						if (EndRow == StartRow-1) {
							if (EndRow == 0)
								return PawnPromotion (StartRow, StartCol, CurrBoard);
							return FillPrevMove(StartRow, StartCol, EndRow, EndCol);
						}
						else
							return DowblePawnMove(StartRow, StartCol, EndRow, EndCol);
				}
				else
					return EnPassant(StartRow, StartCol, EndRow, EndCol, CurrBoard);
			}
			else {
				if (StartCol == EndCol+1 || StartCol == EndCol-1) {
					if (GetColor() == 'W') {
						if (EndRow == StartRow+1) {
							if (EndRow == 7)
								return PawnPromotion (StartRow, StartCol, CurrBoard);
							return FillPrevMove(StartRow, StartCol, EndRow, EndCol);
						}
					}
					else
						if (EndRow == StartRow-1) {
							if (EndRow == 0)
								return PawnPromotion (StartRow, StartCol, CurrBoard);
							return FillPrevMove(StartRow, StartCol, EndRow, EndCol);
						}
				}
			}
			return false;
		}
		bool DowblePawnMove(int StartRow, int StartCol, int EndRow, int EndCol) {
			std::cout << "  @DowblePawnMove" << std::endl;//.....................................COMMENT
			if (!used) {
				if (GetColor() == 'W') {
					if ( EndRow == StartRow+2 )
						return FillPrevMove(StartRow, StartCol, EndRow, EndCol);
				}
				else
					if ( EndRow == StartRow-2 )
						return FillPrevMove(StartRow, StartCol, EndRow, EndCol);
			}
			return false;
		}
		bool EnPassant(int StartRow, int StartCol, int EndRow, int EndCol, std::unique_ptr<CPiece> CurrBoard[8][8]) {
			std::cout << "  @En-Passant." << std::endl;//.....................................COMMENT
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
								return true;
							}
			return false;
		}
		bool PawnPromotion (int StartRow, int StartCol, std::unique_ptr<CPiece> CurrBoard[8][8]) {
			std::cout << "  @PawnPromotion." << std::endl;//.....................................COMMENT
			char NewPiece;
			std::cout << "Enter name of Piece(N/R/B/Q) on which you want to replace your Pawn: ";
			std::cin >> NewPiece;
/*			CurrBoard[StartRow][StartCol] = 0;
			switch (NewPiece) {
				case 'N': CurrBoard[StartRow][StartCol] = new CKnight(color); break;
				case 'R': CurrBoard[StartRow][StartCol] = new CRook(color); break;
				case 'B': CurrBoard[StartRow][StartCol] = new CBishop(color); break;
				case 'Q': CurrBoard[StartRow][StartCol] = new CQueen(color); break;
			} */
			return false;
		}
		bool FillPrevMove(int StartRow, int StartCol, int EndRow, int EndCol) {
			PrevMove[0] = StartRow;
			PrevMove[1] = StartCol;
			PrevMove[2] = EndRow;
			PrevMove[3] = EndCol;
			used = true;
			return true;	
		}
		bool CheckEnPassant (int StartRow, int EndRow, int StartEndCol, std::vector<int> trg) {
			if (trg[0] == StartRow && trg[1] == StartEndCol && trg[2] == EndRow && trg[3] == StartEndCol)
				return true;
			return false;
		}
};


//==================================================================================================================================================
class CKnight : public CPiece { // moves: 'L' like
	public:
		CKnight(char color) : CPiece(color) {};
		~CKnight() {};
	private:
		virtual bool IsUsed() {
			return used;
		}
		virtual char GetPiece() {
			return 'N';
		}
		bool IsLegal(int StartRow, int StartCol, int EndRow, int EndCol, std::unique_ptr<CPiece> CurrBoard[8][8]) {
			// Check that destination is whether empty or enemy field is already done in 'IsLegalMove'
			if (StartCol == EndCol+1 || StartCol == EndCol-1)
				if (StartRow == EndRow+2 || StartRow == EndRow-2)
					return true;
			if (StartCol == EndCol+2 || StartCol == EndCol-2)
				if (StartRow == EndRow+1 || StartRow == EndRow-1)
					return true;
			return false;
		}
};


//==================================================================================================================================================
class CBishop : public CPiece { // moves: DIAGINALS
	public:
		CBishop(char color) : CPiece(color) {};
		~CBishop() {};
	private:
		virtual bool IsUsed() {
			return used;
		}
		virtual char GetPiece() {
			return 'B';
		}
		bool IsLegal(int StartRow, int StartCol, int EndRow, int EndCol, std::unique_ptr<CPiece> CurrBoard[8][8]) {
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
};


//==================================================================================================================================================
class CRook : public CPiece { // moves: VERTICAL or HORIZONRAL
	public:
		CRook(char color) : CPiece(color) {};
		~CRook() {};
	private:
		virtual bool IsUsed() {
			return used;
		}
		virtual char GetPiece() {
			return 'R';
		}
		bool IsLegal(int StartRow, int StartCol, int EndRow, int EndCol, std::unique_ptr<CPiece> CurrBoard[8][8]) {
			//Processing HORIZONRAL move
			if (StartRow == EndRow) {
				int ColOffset = (EndCol - StartCol > 0) ? 1 : -1;
				// Make sure that way is empty
				for (int CheckCol=StartCol+ColOffset ; CheckCol!=EndCol ; CheckCol+=ColOffset) {
					if (CurrBoard[StartRow][CheckCol] != 0) {
						return false;
					}
				}
				used = true;
				return true;
			}
			//Processing VERTICAL move
			else if (EndCol == StartCol) {
				int RowOffset = (EndRow - StartRow > 0) ? 1 : -1;
				// Make sure that way is empty
				for (int CheckRow=StartRow+RowOffset ; CheckRow!=EndRow ; CheckRow+=RowOffset) {
					if (CurrBoard[CheckRow][StartCol] != 0) {
						return false;
					}
				}
				used = true;
				return true;
			}
			return false;
		}
};


//==================================================================================================================================================
class CQueen : public CPiece { // moves: VERTICAL or HORIZONRAL or DIAGONAL
	public:
		CQueen(char color) : CPiece(color) {};
		~CQueen() {};
	private:
		virtual bool IsUsed() {
			return used;
		}
		virtual char GetPiece() {
			return 'Q';
		}
		bool IsLegal(int StartRow, int StartCol, int EndRow, int EndCol, std::unique_ptr<CPiece> CurrBoard[8][8]) {
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
};


//==================================================================================================================================================
class CKing : public CPiece { // moves: ONE adjacent field
	public:
		CKing(char color) : CPiece(color), IsCastlingDone(false) {};
		~CKing() {};
	private:
		virtual bool IsUsed() {
			return used;
		}
		virtual char GetPiece() {
			return 'K';
		}
		bool IsLegal(int StartRow, int StartCol, int EndRow, int EndCol, std::unique_ptr<CPiece> CurrBoard[8][8]) {
			int RowSteps = EndRow - StartRow;
			int ColSteps = EndCol - StartCol;
			if (RowSteps >= -1 && RowSteps <= 1 && ColSteps >= -1 && ColSteps <= 1) {
				used = true;
				return true;
			}
			else if ( RowSteps == 0 && (ColSteps == -2 || ColSteps == 2))
				return Castling(StartRow, StartCol, EndRow, EndCol, CurrBoard);
			return false;
		}
		bool Castling (int StartRow, int StartCol, int EndRow, int EndCol, std::unique_ptr<CPiece> CurrBoard[8][8]) {
			std::cout << "  @Castling" << std::endl;//............................COMMENT
			int CO = (EndCol - StartCol > 0) ? 1 : -1; //Short cut for ColOffset
			//Instant check, to avoid Seg. fault further and was castling used already;
			if ( ((CO == 1 && EndCol+1 > 7) || (CO == -1 && EndCol-1 < 0)) && IsCastlingDone == false )
				return false;
			//On destination and adjacent field should be nothing;
			if (!used && CurrBoard[EndRow][EndCol] == 0 && CurrBoard[StartRow][StartCol+(1*CO)] == 0) {
				//Is there something right after destination?;
				if ( CurrBoard[EndRow][EndCol+(1*CO)] != 0) {
					//Is it a COok?
					if (CurrBoard[EndRow][EndCol+(1*CO)]->GetPiece() == 'R' && CurrBoard[EndRow][EndCol+(1*CO)]->GetColor() == color) {
						if (CurrBoard[EndRow][EndCol+(1*CO)]->IsUsed())
							return false;
						CurrBoard[EndRow][EndCol-(1*CO)] = std::move(CurrBoard[EndRow][EndCol+(1*CO)]);
						CurrBoard[EndRow][EndCol+(1*CO)] = 0;
						IsCastlingDone = true;
						used = true;
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
							if (CurrBoard[EndRow][EndCol+(2*CO)]->IsUsed())
								return false;
							CurrBoard[EndRow][EndCol-(1*CO)] = std::move(CurrBoard[EndRow][EndCol+(2*CO)]);
							CurrBoard[EndRow][EndCol+(2*CO)] = 0;
							IsCastlingDone = true;
							used = true;
							return true;
						}
				}
			}
			return false;
		}
		bool IsCastlingDone;
};


//==================================================================================================================================================
class CBoard {
	public:
		CBoard() {
			for (int Row = 0; Row < 8; ++Row)
				for (int Col = 0; Col < 8; ++Col)
					board[Row][Col] = 0;
		// Allocate and place black pieces
		for (int Col = 0; Col < 8; ++Col)
			board[6][Col] = std::unique_ptr<CPiece>(new CPawn('B'));
		board[7][0] = std::unique_ptr<CPiece>(new CRook('B'));
		board[7][1] = std::unique_ptr<CPiece>(new CKnight('B'));
		board[7][2] = std::unique_ptr<CPiece>(new CBishop('B'));
		board[7][3] = std::unique_ptr<CPiece>(new CKing('B'));
		board[7][4] = std::unique_ptr<CPiece>(new CQueen('B'));
		board[7][5] = std::unique_ptr<CPiece>(new CBishop('B'));
		board[7][6] = std::unique_ptr<CPiece>(new CKnight('B'));
		board[7][7] = std::unique_ptr<CPiece>(new CRook('B'));
		// Allocate and place white pieces
		for (int Col = 0; Col < 8; ++Col)
			board[1][Col] = std::unique_ptr<CPiece>(new CPawn('W'));
		board[0][0] = std::unique_ptr<CPiece>(new CRook('W'));
		board[0][1] = std::unique_ptr<CPiece>(new CKnight('W'));
		board[0][2] = std::unique_ptr<CPiece>(new CBishop('W'));
		board[0][3] = std::unique_ptr<CPiece>(new CKing('W'));
		board[0][4] = std::unique_ptr<CPiece>(new CQueen('W'));
		board[0][5] = std::unique_ptr<CPiece>(new CBishop('W'));
		board[0][6] = std::unique_ptr<CPiece>(new CKnight('W'));
		board[0][7] = std::unique_ptr<CPiece>(new CRook('W')); 

//board[7][7] = std::unique_ptr<CPiece>(new CKing('B'));//.....................................COMMENT
		}
		~CBoard() {
			for (int Row = 0; Row < 8; ++Row) {
				for (int Col = 0; Col < 8; ++Col) {
					//delete board[Row][Col]; deleting object? I like dat.
					board[Row][Col] = 0;
				}
			}
		}
		void Print (void) {
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
		bool IsInCheck (char color) {
			std::cout << "  @IsInCheck." << std::endl;//.....................................COMMENT
			int KingRow;
			int KingCol;
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
		bool IsPossibleMoves(char color) {
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
										std::unique_ptr<CPiece> tmp	= std::move(board[EndRow][EndCol]);
										board[EndRow][EndCol]	= std::move(board[StartRow][StartCol]);
										board[StartRow][StartCol]	= 0;
										bool FullValidMove = !IsInCheck(color);
										// Undo the move
										board[StartRow][StartCol] = std::move(board[EndRow][EndCol]);
										board[EndRow][EndCol]	= std::move(tmp);
										if (FullValidMove) {
											std::cout << " @can move from[" << StartRow+1 << ":" << StartCol+1 << "] to [" << EndRow+1 << ":" << EndCol+1  << "]" << std::endl;
											return true;
										}
									}
			return false;
		}
		//CPiece* board[8][8];
		std::unique_ptr<CPiece> board[8][8];
};


//==================================================================================================================================================
class CChess {
	public:
		CChess() : Turn('W') {}
		~CChess() {}
		void Start() {
			do {
				GetNextMove(GameBoard.board);
				SwitchTurn();
			} while (!GameIsOver());
			GameBoard.Print();
		}
		void GetNextMove(std::unique_ptr<CPiece> board[8][8]) {
			//flag shows was move valid or no. if no, loop take place.
			bool IsValid=false; 
			do {
				GameBoard.Print();
				std::cout << Turn <<  "`s moves: ";
				int from;
				std::cin >> from;
				int StartRow = (from / 10) - 1;
				int StartCol = (from % 10) - 1;
				std::cout << "       To: ";
				int to;
				std::cin >> to;
				int EndRow = (to / 10) - 1;
				int EndCol = (to % 10) - 1;
				//check coordinates.
				if ( StartRow<=7 && StartRow>=0 && StartCol<=7 && StartCol>=0 &&
						 EndRow<=7   && EndRow>=0   && EndCol<=7   && EndCol>=0 ) {
					std::cout << " @Coordinates is OK" << std::endl;//.....................................COMMENT
					std::unique_ptr<CPiece> currP = std::move(board[StartRow][StartCol]);
					//check is piese is choosen correctly.
					if (currP != 0 && currP->GetColor() == Turn) {
						std::cout << " @You choose correct piece" << std::endl;//.....................................COMMENT
						//is dat valid move?
						std::cout << " @Is legal" << std::endl;//.....................................COMMENT
						if ( currP->IsLegalMove(StartRow, StartCol, EndRow, EndCol, board) ) {
							//all is Ok, lets make a move.
							std::unique_ptr<CPiece> tmp	= std::move(board[EndRow][EndCol]); //remember Poece which is taken;
							board[EndRow][EndCol] = std::move(board[StartRow][StartCol]);
							board[StartRow][StartCol] = 0;
							//need to make additional check here(is piece is in check).
							IsValid=true;
							if (GameBoard.IsInCheck(Turn)) {
								std::cout << " @Check! Protect King!" << std::endl;//.....................................COMMENT
								IsValid=false;
								//undo last move
								board[StartRow][StartCol] = std::move(board[EndRow][EndCol]);
								board[EndRow][EndCol] = std::move(tmp);
							}
						}
					}
				}
				if (!IsValid)
					std::cout << "Invalid move! " << std::endl;
			} while (!IsValid);
		}
		void SwitchTurn() {
			Turn = (Turn == 'W') ? 'B' : 'W';
		}
		bool GameIsOver(void) {
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
	private:
		CBoard GameBoard;
		char Turn;
};


int main () {

CChess chess;
chess.Start();

return 0;
}


