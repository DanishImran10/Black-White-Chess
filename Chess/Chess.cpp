#include<fstream>
#include<iostream>
#include<cstring>
#include<Windows.h>
#define BLACK 0
#define BROWN 6
#define WHITE 15
#define GREEN 2
#define RED 4
#define LBLUE 9

using namespace std;
int white = 0, black = 1;
int dim = 8;

void Load(char B[][8], string fn)
{
	ifstream Rdr(fn);

	for (int r = 0; r < 8; r++)
	{
		for (int c = 0; c < 8; c++)
		{
			Rdr >> B[r][c];
		}
	}
}

void Save(char B[][8], string fn)
{
	ofstream Writer(fn);

	for (int r = 0; r < 8; r++)
	{
		for (int c = 0; c < 8; c++)
		{
			Writer << B[r][c];
		}
		Writer << endl;
	}
}

void SetClr(int tcl, int bcl)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (tcl + (bcl * 16)));
}


void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}


void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}






void init(char board[][8], int& turn, string Ns[])
{
	for (int r = 0; r < dim; r++)
	{
		for (int c = 0; c < dim; c++)
		{
			if (r > 1 && r < 6)
			{
				board[r][c] = '-';
			}
			else
				if (r == 1)
				{
					board[r][c] = 'p';
				}
				else
					if (r == 6)
					{
						board[r][c] = 'P';
					}
		}
	}
	int r = 0;
	board[r][0] = 'r';
	board[r][1] = 'h';
	board[r][2] = 'b';
	board[r][3] = 'q';
	board[r][4] = 'k';
	board[r][5] = 'b';
	board[r][6] = 'h';
	board[r][7] = 'r';
	r = 7;
	board[r][0] = 'R';
	board[r][1] = 'H';
	board[r][2] = 'B';
	board[r][3] = 'Q';
	board[r][4] = 'K';
	board[r][5] = 'B';
	board[r][6] = 'H';
	board[r][7] = 'R';

	cout << "Enter player 1 name: " << "\n";
	cin >> Ns[0];
	cout << "Enter player 2 name: " << "\n";
	cin >> Ns[1];

	turn = 0;

}

void printboard(char board[][8])
{
	system("cls");
	cout << ' ' << "\t" << "A" << "\t" << "B" << "\t" << "C" << "\t" << "D" << "\t" << "E" << "\t" << "F" << "\t" << "G" << "\t" << "H" << "\n\n";
	for (int r = 0; r < dim; r++)
	{
		cout << r + 1;
		for (int c = 0; c < dim; c++)
		{
			cout << "\t" << board[r][c];
		}
		cout << "\n\n";
	}
}

void turnmsg(string Ns[], int turn)
{
	gotoRowCol(18, 0);
	cout << Ns[turn] << "'s turn: \n";
}



bool IsWhitePiece(char p)
{
	return p >= 'A' && p <= 'Z';
}


bool IsBlackPiece(char p)
{
	return p >= 'a' && p <= 'z';
}

bool IsMyPiece(char p, int turn)
{
	if (turn == white)
	{
		return IsWhitePiece(p);
	}
	return IsBlackPiece(p);
}

bool IsValidSource(char board[][8], int turn, int sr, int sc)
{
	return sc >= 0 && sc <= 7 && sr >= 0 && sr <= 7 && IsMyPiece(board[sr][sc], turn);
}


void coordset1(char board[][8], int turn, int& sc, int& sr)
{
	do
	{
		getRowColbyLeftClick(sr, sc);
		sr = (sr / 2) - 1;
		sc = (sc / 8) - 1;
	} while (!IsValidSource(board, turn, sr, sc));
}


bool IsValidDest(char board[][8], int turn, int dr, int dc)
{
	return dc >= 0 && dc <= 7 && dr >= 0 && dr <= 7 && !IsMyPiece(board[dr][dc], turn);
}

void coordset2(char board[][8], int turn, int& dc, int& dr)
{
	do
	{
		getRowColbyLeftClick(dr, dc);
		dr = (dr / 2) - 1;
		dc = (dc / 8) - 1;
	} while (!IsValidDest(board, turn, dr, dc));
}

void move(char board[][8], int sr, int sc, int dr, int dc)
{
	board[dr][dc] = board[sr][sc];
	board[sr][sc] = '-';
}

int turnchange(int turn)
{
	turn = (turn + 1) % 2;
	return turn;
}

bool IsHorizontalMove(int sr, int sc, int dr, int dc)
{
	return sr == dr;
}

bool IsVerticalMove(int sr, int sc, int dr, int dc)
{
	return sc == dc;
}

int abs(int x)
{
	if (x < 0)
	{
		x *= -1;
	}
	return x;
}

bool IsDiagMove(int sr, int sc, int dr, int dc)
{
	int DR = abs(sr - dr);
	int SR = abs(sc - dc);
	return SR == DR;
}

bool IsHorizontalPathClear(char board[][8], int sr, int sc, int dr, int dc)
{
	if (sc < dc)
	{
		for (int c = sc + 1; c < dc; c++)
		{
			if (board[sr][c] != '-')
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		for (int c = sc - 1; c > dc; c--)
		{
			if (board[sr][c] != '-')
			{
				return false;
			}
		}
		return true;
	}
}

bool IsVerticalPathClear(char board[][8], int sr, int sc, int dr, int dc)
{
	if (sr < dr)
	{
		for (int r = sr + 1; r < dr; r++)
		{
			if (board[r][sc] != '-')
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		for (int r = sr - 1; r > dr; r--)
		{
			if (board[r][sc] != '-')
			{
				return false;
			}
		}
		return true;
	}
}

bool IsDiagPathClear(char board[][8], int sr, int sc, int dr, int dc)
{
	int t = abs(dr - sr);
	int DR = dr - sr;
	int DC = dc - sc;

	if (DR < 0 && DC < 0)
	{
		for (int i = 1; i < t; i++)
		{
			if (board[sr - i][sc - i] != '-')
			{
				return false;
			}
		}
		return true;
	}
	else
		if (DR > 0 && DC > 0)
		{
			for (int i = 1; i < t; i++)
			{
				if (board[sr + i][sc + i] != '-')
				{
					return false;
				}
			}
			return true;
		}
		else
			if (DR > 0 && DC < 0)
			{
				for (int i = 1; i < t; i++)
				{
					if (board[sr + i][sc - i] != '-')
					{
						return false;
					}
				}
				return true;
			}
			else
			{
				for (int i = 1; i < t; i++)
				{
					if (board[sr - i][sc + i] != '-')
					{
						return false;
					}
				}
				return true;
			}
}

bool IsRookLegal(char board[][8], int sr, int sc, int dr, int dc)
{
	return (IsHorizontalMove(sr, sc, dr, dc) && IsHorizontalPathClear(board, sr, sc, dr, dc)) || (IsVerticalMove(sr, sc, dr, dc) && IsVerticalPathClear(board, sr, sc, dr, dc));
}

bool IsBishopLegal(char board[][8], int sr, int sc, int dr, int dc)
{
	return IsDiagMove(sr, sc, dr, dc) && IsDiagPathClear(board, sr, sc, dr, dc);
}

bool IsQueenLegal(char board[][8], int sr, int sc, int dr, int dc)
{
	return (IsHorizontalMove(sr, sc, dr, dc) && IsHorizontalPathClear(board, sr, sc, dr, dc)) || (IsVerticalMove(sr, sc, dr, dc) && IsVerticalPathClear(board, sr, sc, dr, dc)) || (IsDiagMove(sr, sc, dr, dc) && IsDiagPathClear(board, sr, sc, dr, dc));
}

bool IsKingLegal(char board[][8], int sr, int sc, int dr, int dc)
{
	int DR = abs(sr - dr);
	int DC = abs(sc - dc);
	bool a = IsVerticalPathClear(board, sr, sc, dr, dc) || IsHorizontalPathClear(board, sr, sc, dr, dc) || IsDiagPathClear(board, sr, sc, dr, dc);
	return DR <= 1 && DC <= 1 && a;
}

bool IsHorseLegal(char board[][8], int sr, int sc, int dr, int dc)
{
	int DR = abs(sr - dr);
	int DC = abs(sc - dc);
	return (DR == 1 && DC == 2) || (DR == 2 && DC == 1);
}


bool IsPawnKillLegal1(char board[][8], int sr, int sc, int dr, int dc)
{
	int DR = sr - dr;
	int DC = sc - dc;
	return (DR == 1 && DC == 1 && IsBlackPiece(board[dr][dc])) || (DR == 1 && DC == -1 && IsBlackPiece(board[dr][dc]));
}


bool IsPawnLegal1(char board[][8], int sr, int sc, int dr, int dc)
{
	int DR = sr - dr;
	int DC = sc - dc;
	bool a = (DR == 1 && DC == 0) || (DR == 2 && DC == 0 && IsVerticalPathClear(board, sr, sc, dr, dc));
	bool b = (DR == 1 && DC == 0);
	if (sr == 6)
	{
		return (a && !IsBlackPiece(board[dr][dc])) || IsPawnKillLegal1(board, sr, sc, dr, dc);
	}
	return (b && !IsBlackPiece(board[dr][dc])) || IsPawnKillLegal1(board, sr, sc, dr, dc);
}

bool IsPawnKillLegal2(char board[][8], int sr, int sc, int dr, int dc)
{
	int DR = dr - sr;
	int DC = dc - sc;
	return (DR == 1 && DC == 1 && IsWhitePiece(board[dr][dc])) || (DR == 1 && DC == -1 && IsWhitePiece(board[dr][dc]));
}

bool IsPawnLegal2(char board[][8], int sr, int sc, int dr, int dc)
{
	int DR = dr - sr;
	int DC = dc - sc;
	bool a = (DR == 1 && DC == 0) || (DR == 2 && DC == 0 && IsVerticalPathClear(board, sr, sc, dr, dc));
	bool b = (DR == 1 && DC == 0);
	if (sr == 1)
	{
		return (a && !IsWhitePiece(board[dr][dc])) || IsPawnKillLegal2(board, sr, sc, dr, dc);
	}
	return (b && !IsWhitePiece(board[dr][dc])) || IsPawnKillLegal2(board, sr, sc, dr, dc);
}




bool IsLegalMove(char board[][8], int sr, int sc, int dr, int dc)
{
	switch (board[sr][sc])
	{
	case 'r':
	case 'R':
		return IsRookLegal(board, sr, sc, dr, dc);

	case 'b':
	case 'B':
		return IsBishopLegal(board, sr, sc, dr, dc);

	case 'q':
	case 'Q':
		return IsQueenLegal(board, sr, sc, dr, dc);

	case 'k':
	case 'K':
		return IsKingLegal(board, sr, sc, dr, dc);

	case 'h':
	case 'H':
		return IsHorseLegal(board, sr, sc, dr, dc);

	case 'p':
		return IsPawnLegal2(board, sr, sc, dr, dc);

	case 'P':
		return IsPawnLegal1(board, sr, sc, dr, dc);
	}
}

void findKing(char board[][8], int turn, int& kr, int& kc)
{
	char ks = (turn == white) ? 'K' : 'k';
	for (int r = 0; r < dim; r++)
	{
		for (int c = 0; c < dim; c++)
		{
			if (IsMyPiece(board[r][c], turn) && board[r][c] == ks)
			{
				kr = r;
				kc = c;
				return;
			}
		}
	}
}

bool Check(char board[][8], int turn)
{
	int kr, kc;
	turn = turnchange(turn);
	findKing(board, turn, kr, kc);
	turn = turnchange(turn);
	for (int r = 0; r < dim; r++)
	{
		for (int c = 0; c < dim; c++)
		{
			if (IsMyPiece(board[r][c], turn) && IsLegalMove(board, r, c, kr, kc))
			{
				return true;
			}
		}
	}
	return false;
}

bool selfCheck(char board[][8], char copy[][8], int turn, int sr, int sc, int dr, int dc)
{
	for (int r = 0; r < dim; r++)
	{
		for (int c = 0; c < dim; c++)
		{
			copy[r][c] = board[r][c];
		}
	}
	move(copy, sr, sc, dr, dc);
	turn = turnchange(turn);
	return Check(copy, turn);
}

bool CanIMoveAPiece(char board[][8], char copy[][8], int turn)
{
	for (int sr = 0; sr < dim; sr++)
	{
		for (int sc = 0; sc < dim; sc++)
		{
			for (int dr = 0; dr < dim; dr++)
			{
				for (int dc = 0; dc < dim; dc++)
				{
					if (IsMyPiece(board[sr][sc], turn) && IsValidDest(board, turn, dr, dc) && IsLegalMove(board, sr, sc, dr, dc))
					{
						if (!selfCheck(board, copy, turn, sr, sc, dr, dc))
						{
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

bool Checkmate(char board[][8], char copy[][8], int turn)
{
	return !CanIMoveAPiece(board, copy, !turn) && Check(board, turn);
}

bool Stalemate(char board[][8], char copy[][8], int turn)
{
	return !CanIMoveAPiece(board, copy, !turn) && !Check(board, turn);
}




void highlight(char board[][8], char duplicate[][8], char copy[][8], int turn, int sr, int sc, int dr, int dc)
{
	for (int r = 0; r < dim; r++)
	{
		for (int c = 0; c < dim; c++)
		{
			if (IsValidDest(board, turn, r, c) && IsLegalMove(board, sr, sc, r, c) && !selfCheck(board, copy, turn, sr, sc, r, c))
			{
				duplicate[r][c] = 'X';
				gotoRowCol((r * 2) + 2, (c * 8) + 8);
				cout << 'X';
			}
			else
				duplicate[r][c] = '-';
		}
	}
}

void unhighlight(char board[][8], char duplicate[][8], int sr, int sc)
{
	for (int r = 0; r < dim; r++)
	{
		for (int c = 0; c < dim; c++)
		{
			if (duplicate[r][c] == 'X' || (r == sr && c == sc))
			{
				gotoRowCol((r * 2) + 2, (c * 8) + 8);
				cout << board[r][c];
			}
		}
	}
}

void PawnPromotion(char board[][8], int turn, int dr, int dc)
{
	char pawnconvert;
	if (turn == white)
	{
		if (dr == 0)
		{
			while (1)
			{
				gotoRowCol(18, 0);
				cout << "What do you want your pawn to convert into?";
				cin >> pawnconvert;
				gotoRowCol(18, 0);
				cout << "                                                ";
				switch (pawnconvert)
				{
				case 'R':
					board[dr][dc] = 'R';
					gotoRowCol((dr * 2) + 2, (dc * 8) + 8);
					cout << board[dr][dc];
					break;
				case 'H':
					board[dr][dc] = 'H';
					gotoRowCol((dr * 2) + 2, (dc * 8) + 8);
					cout << board[dr][dc];
					break;
				case 'B':
					board[dr][dc] = 'B';
					gotoRowCol((dr * 2) + 2, (dc * 8) + 8);
					cout << board[dr][dc];
					break;
				case 'Q':
					board[dr][dc] = 'Q';
					gotoRowCol((dr * 2) + 2, (dc * 8) + 8);
					cout << board[dr][dc];
					break;
				default:
					gotoRowCol(20, 0);
					cout << "Wrong Input!";
					gotoRowCol(20, 0);
					cout << "            ";
					continue;
				}
				break;
			}

		}
	}
	else
	{
		if (dr == 7)
		{
			while (1)
			{
				gotoRowCol(20, 0);
				cout << "What do you want your pawn to convert into?";
				cin >> pawnconvert;
				gotoRowCol(20, 0);
				cout << "                                             ";
				switch (pawnconvert)
				{
				case 'r':
					board[dr][dc] = 'r';
					gotoRowCol((dr * 2) + 2, (dc * 8) + 8);
					cout << board[dr][dc];
					break;
				case 'h':
					board[dr][dc] = 'h';
					gotoRowCol((dr * 2) + 2, (dc * 8) + 8);
					cout << board[dr][dc];
					break;
				case 'b':
					board[dr][dc] = 'b';
					gotoRowCol((dr * 2) + 2, (dc * 8) + 8);
					cout << board[dr][dc];
					break;
				case 'q':
					board[dr][dc] = 'q';
					gotoRowCol((dr * 2) + 2, (dc * 8) + 8);
					cout << board[dr][dc];
					break;
				default:
					gotoRowCol(20, 0);
					cout << "Wrong Input!";
					gotoRowCol(20, 0);
					cout << "            ";
					continue;
				}
				break;
			}
		}
	}
}



int main()
{
	char board[8][8], duplicate[8][8], copy[8][8];
	int turn, sr = 0, sc = 0, dr = 0, dc = 0;
	string Ns[2];
	char game;

	init(board, turn, Ns);
	cout << "Do you want to play a new game or a saved game: (n/s)";
	cin >> game;
	if (game == 's')
	{
		Load(board, "savedgame.txt");
	}
	printboard(board);
	do
	{
		int count = 0, g = 19;

		do
		{
			if (count > 0)
			{
				gotoRowCol(g, 0);
				cout << "Wrong Input!" << "\n";
				system("pause");
				gotoRowCol(g, 0);
				cout << "            " << "\n" << "                                               ";
				g++;
			}

			turnmsg(Ns, turn);
			coordset1(board, turn, sc, sr);
			highlight(board, duplicate, copy, turn, sr, sc, dr, dc);
			coordset2(board, turn, dc, dr);
			unhighlight(board, duplicate, sr, sc);
			count++;
		} while (!IsLegalMove(board, sr, sc, dr, dc) || selfCheck(board, copy, turn, sr, sc, dr, dc));
		g = 21;
		while (count > 0)
		{
			gotoRowCol(g, 0);
			cout << "                    " << "\n";
			g++;
			count--;
		}

		move(board, sr, sc, dr, dc);
		unhighlight(board, duplicate, sr, sc);

		if (board[dr][dc] == 'p' || board[dr][dc] == 'P')
		{
			PawnPromotion(board, turn, dr, dc);
		}

		Save(board, "savedgame.txt");

		if (Check(board, turn))
		{
			gotoRowCol(22, 0);
			cout << "Check:" << "\n";
			system("pause");
			gotoRowCol(22, 0);
			cout << "          " << "\n" << "                                 ";
		}

		turn = turnchange(turn);
	} while (CanIMoveAPiece(board, copy, turn));

	turn = turnchange(turn);
	gotoRowCol(25, 0);
	if (Checkmate(board, copy, turn))
	{
		cout << "Checkmate!" << "\n" << Ns[turn] << " has won the game.";
		return 0;
	}
	if (Stalemate(board, copy, turn))
	{
		cout << "The game has ended in a stalemate.";
		return 0;
	}
}
