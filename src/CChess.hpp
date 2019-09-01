#include <iostream>
#include "CBoard.hpp"
#include <string>
#include <vector>
#ifndef CChess_hpp
#define CChess_hpp

/*!
\file Declaration part of CChess class.
*/

/*!
\brief Main and higher class.

The very basic class from which all stuff is started.
Variable of exactly this class gives us the actual opportunity to start the game.
Covers the main cases and actions of the game itself.
*/
class CChess {
	public:
		CChess(char* _SafeFile) : SafeFile(_SafeFile), Turn('W') {}
		~CChess() {}
		void Start();
		bool FileIsValid(const char* SafeFile);
		void FillDefPositions (std::vector<std::string>& PiecesPossitions);
		bool ReadFileToVector (const char* SafeFile, std::vector<std::string>& PiecesPossitions);
		void GetNextMove(CPiece* board[8][8]);
		int ScanMove (int& Row, int& Col, CPiece* CurrBoard[8][8]);
		/*!
		Check whether string is containing only digits.
		\param str string to check.
		\return 'true'-contain, 'false'-not
		*/
		bool IsDigit(const std::string &str) {
	    return str.find_first_not_of("0123456789") == std::string::npos; }
		void Save(CPiece* CurrBoard[8][8]);
		bool Exist (const std::string name);
		void PawnPromotion(const int StartRow, const int StartCol, const int EndRow, const int EndCol, CPiece* CurrBoard[8][8]);
		void PawnPromotionHelper(const int StartRow, const int StartCol, CPiece* CurrBoard[8][8], char color);
		///\details Switch the turn of player.
		void SwitchTurn(void) {
			Turn = (Turn == 'W') ? 'B' : 'W'; }
		bool GameIsOver(void);
	private:
		char* SafeFile; ///< Store name of load file(or NULL, if it is not specified).
		CBoard GameBoard; ///< Ours 8x8 game board.
		char Turn; ///< Carry current Players turn(W or B).
};
#endif /* CChess_hpp */
