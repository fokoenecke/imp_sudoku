#include "SudokuProblem.h"

SudokuProblem::SudokuProblem(int initial[81])
{
	for (int i = 0; i < 81; i++)
	{
		working[i] = initial[i];
	}
}


SudokuProblem::~SudokuProblem()
{
}

void SudokuProblem::print()
{
	// print initial array
	for (int i = 0; i < 81; i++)
	{
		// print value of each cell
		if (working[i] == 0)
		{
			cout << "_ ";
		}
		else
		{
			cout << working[i] << " ";
		}

		// divide blocks vertically
		if ((i + 1) % 3 == 0)
		{
			cout << " ";
		}
		// break at end of line
		if ((i + 1) % 9 == 0)
		{
			cout << endl;
		}
		// divide blocks horizontally
		if ((i + 1) % 27 == 0)
		{
			cout << endl;
		}
	}

	cout << endl;
}

int* SudokuProblem::solve()
{
	cout << "fange an, das sudoku problem zu loesen..." << endl;
	// ein array, dass eine liste der noch möglichen zahlen für das feld enthält
	list<int> choices[81];
	//list<int> initial = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	for (int i = 0; i < 81; i++)
	{
		choices[i] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	}

	// true, wenn das feld abgearbeitet wurde
	bool finished[81];
	for (int i = 0; i < 81; i++)
	{
		finished[i] = false;
	}
	
	while (true)
	{
		cout << "loesche moeglichkeiten..." << endl;
		// lösche möglichkeiten
		for (int f = 0; f < 81; f++)
		{
			// wenn eine zahl im feld steht und es noch nicht abgearbeitet ist
			if (working[f] != 0 && finished[f] == false)
			{
				// lösche möglichkeiten in der zeile
				for (int z = 0; z < 9; z++)
				{
					int feld = f / 9 * 9 + z;
					// wenn in dem feld noch keine zahl eingetragen ist
					if (working[feld] == 0)
					{
						// streiche die möglichkeit
						choices[feld].remove(working[f]);
					}
				}

				// lösche möglichkeiten in der spalte
				for (int s = 0; s < 9; s++)
				{
					int feld = 9 * s + f % 9;
					// wenn in dem feld noch keine zahl eingetragen ist
					if (working[feld] == 0)
					{
						// streiche die möglichkeit
						choices[feld].remove(working[f]);
					}
				}

				// lösche möglichkeiten in dem block
				for (int by = 0; by < 3; by++)
				{
					for (int bx = 0; bx < 3; bx++)
					{
						int feld = (f / 3 * 3 + bx) + (9 * (by - ((f / 9) % 3)));
						// wenn in dem feld noch keine zahl eingetragen ist
						if (working[feld] == 0)
						{
							// streiche die möglichkeit
							choices[feld].remove(working[f]);
						}
					}
				}

				// markiere als bearbeitet
				finished[f] = true;
			}
		}
		cout << "finde eindeutige loesungen..." << endl;
		for (int f = 0; f < 81; f++)
		{
			// wenn nur eine möglichkeit besteht, ist dies die lösung
			if (choices[f].size() == 1)
			{
				working[f] = choices[f].front();
				choices[f].pop_front();
				cout << working[f] << " ist die loesung fuer feld " << f << endl;
			}
		}

		cout << "suche nach weiteren loesungen..." << endl;
		
		cout << "... in den zeilen:" << endl;
		int counterZ[9][9]; // zeile x zahl
		int lastSeenZ[9][9]; // zeile x zahl
		for (int z = 0; z < 9; z++) //durchlaufe die zeilen
		{
			// und zähle in der zeile, wie oft eine moeglichkeit vorkommt
			for (int s = 0; s < 9; s++)
			{
				int feld = z * 9 + s;
				// durchlaufe die moeglichkeiten des feldes
				for each (int c in choices[feld])
				{
					// und zaehle den counter fuer die zahl hoch
					counterZ[z][c - 1]++;
					// und merke, wo die zahl das letzte mal gesehen wurde
					lastSeenZ[z][c - 1] = feld;
				}
			}
		}
		// dann gucken, wo in einer zeile eine moeglichkeit nur 1x vorkommt
		bool isSingleZ[9][9]; // zeile x zahl
		for (int z = 0; z < 9; z++) // durchlaufe die zeilen
		{
			for (int c = 0; c < 9; c++) // und die counter
			{
				// wenn so ein counter jetzt 1 ist, kommt die zahl nur 1x in der zeile vor
				if (counterZ[z][c] == 1)
				{
					// und man merkt sie sich
					isSingleZ[z][c] = true;
				}
			}
		}
		// single durchlaufen, gucken wo sie als letztes vorkamen und das als loesung festsetzen
		for (int z = 0; z < 9; z++) // durchlaufe die zeilen
		{
			for (int n = 0; n < 9; n++) // durchlaufe die zahlen
			{
				// wenn single
				if (isSingleZ[z][n] == true)
				{
					int f = lastSeenZ[z][n];
					// setze die loesung fuer das feld, wo sie zuletzt gesehen wurde
					working[f] = n + 1;
					choices[f].remove(n + 1);
					cout << working[f] << " ist die loesung fuer feld " << f << endl;
				}
			}
		}

		cout << "... in den spalten:" << endl;
		int counterS[9][9]; // spalte x zahl
		int lastSeenS[9][9]; // spalte x zahl
		for (int s = 0; s < 9; s++) // durchlaufe die spalten
		{
			// und zähle in der spalte, wie oft eine moeglichkeit vorkommt
			for (int z = 0; z < 9; z++)
			{
				int feld = z * 9 + s;
				// durchlaufe die moeglichkeiten des feldes
				for each (int c in choices[feld])
				{
					// und zaehle den counter fuer die zahl hoch
					counterS[z][c - 1]++;
					// und merke, wo die zahl das letzte mal gesehen wurde
					lastSeenS[z][c - 1] = feld;
				}
			}
		}
		// dann gucken, wo in einer spalte eine moeglichkeit nur 1x vorkommt
		bool isSingleS[9][9]; // spalte x zahl
		for (int s = 0; s < 9; s++) // durchlaufe die spalten
		{
			for (int c = 0; c < 9; c++) // und die counter
			{
				// wenn so ein counter jetzt 1 ist, kommt die zahl nur 1x in der spalte vor
				if (counterS[s][c] == 1)
				{
					// und man merkt sie sich
					isSingleS[s][c] = true;
				}
			}
		}
		// single durchlaufen, gucken wo sie als letztes vorkamen und das als loesung festsetzen
		for (int s = 0; s < 9; s++) // durchlaufe die spalten
		{
			for (int n = 0; n < 9; n++) // durchlaufe die zahlen
			{
				// wenn single
				if (isSingleS[s][n] == true)
				{
					int f = lastSeenS[s][n];
					// setze die loesung fuer das feld, wo sie zuletzt gesehen wurde
					working[f] = n + 1;
					choices[f].remove(n + 1);
					cout << working[f] << " ist die loesung fuer feld " << f << endl;
				}
			}
		}

		cout << "... in den bloecken:" << endl;
		int counterB[9][9]; // block x zahl
		int lastSeenB[9][9]; // block x zahl
		for (int bx = 0; bx < 3; bx++) // durchlaufe die blöcke horizontal
		{
			for (int by = 0; by < 3; by++) // und vertikal
			{
				for (int z = 0; z < 3; z++) // durchlaufe die zeilen des blocks
				{
					for (int s = 0; s < 3; s++) // durchlaufe die spalten des blocks
					{
						//int feld = (f / 3 * 3 + bx) + (9 * (by - ((f / 9) % 3)));
						int feld = (3 * bx) + (27 * by) + (9 * z) + s;
						// durchlaufe die moeglichkeiten des feldes
						for each (int c in choices[feld])
						{
							// und zaehle den counter fuer die zahl hoch
							counterB[(3 * bx) + by][c - 1]++;
							// und merke, wo die zahl das letzte mal gesehen wurde
							lastSeenB[(3 * bx) + by][c - 1] = feld;
						}
					}
				}
			}
		}
		// dann gucken, wo in einem block eine moeglichkeit nur 1x vorkommt
		bool isSingleB[9][9]; // block x zahl
		for (int b = 0; b < 9; b++) // durchlaufe die blöcke
		{
			for (int c = 0; c < 9; c++) // und die counter
			{
				// wenn so ein counter jetzt 1 ist, kommt die zahl nur 1x in dem block vor
				if (counterB[b][c] == 1)
				{
					// und man merkt sie sich
					isSingleB[b][c] = true;
				}
			}
		}
		// single durchlaufen, gucken wo sie als letztes vorkamen und das als loesung festsetzen
		for (int b = 0; b < 9; b++) // durchlaufe die blöcke
		{
			for (int n = 0; n < 9; n++) // durchlaufe die zahlen
			{
				// wenn single
				if (isSingleB[b][n] == true)
				{
					int f = lastSeenB[b][n];
					// setze die loesung fuer das feld, wo sie zuletzt gesehen wurde
					working[f] = n + 1;
					choices[f].remove(n + 1);
					cout << working[f] << " ist die loesung fuer feld " << f << endl;
				}
			}
		}

		print();

		cout << "ueberpruefe ob alle felder ausgefuellt sind..." << endl;
		bool complete = true;
		for (int f = 0; f < 81; f++)
		{
			if (working[f] == 0)
			{
				complete = false;
			}
		}
		if (complete)
		{
			cout << "alle felder sind ausgefuellt." << endl;
			return working;
		}
		else
		{
			cout << "weiter im text..." << endl;
		}

		cout << endl;
	}
}
