#include "SudokuProblem.h"

SudokuProblem::SudokuProblem(int* initial) {
	// Füllen des Sudokus aus dem übergebenen Array
	cout << initial[0] << endl;
	sudoku.resize(9);
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			sudoku[i].push_back(initial[i * 9 + j]);
		}
	}
	cout << "Initiales Sudoku Problem: " << endl;
	print();
	// Füllen des Möglichkeitsarrays
	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {
			choices[row][col] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		}
	}
}

SudokuProblem::~SudokuProblem() {}

void SudokuProblem::solve()
{
	// Wenn das Sudoku noch nicht geloest wurde:
	if (!isSolved()) {
		// Bereite das Sudoku vor (löst ganz einfache Sudokus bereits)
		solvePreparation();
		// Starte rekursives Lösen (für schwierigere Sudokus)
		solveRecursion(0, 0);
		// Ausgabe der Informationen zur Lösung des Sudokus:
		if (isSolved()) {
			cout << "Sudoku Problem wurde geloest: " << endl;
			print();
			cout << "Es waren " << recursionCounter << " wiederholte rekursive Aufrufe noetig." << endl;
		}
		else {
			cout << "Das Sudoku Problem wurde nicht geloest." << endl;
		}
	}
}

bool SudokuProblem::solveRecursion(int row, int col) {
	recursionCounter++;
	// Wenn das Soduko bereits gelöst ist, gebe true zurück
	if (isSolved()) return true;
	// Gehe zum ersten leeren Feld:
	// Solange wir nicht alle Zeilen durchlaufen haben
	// und wir nicht auf einem leeren Feld sind
	while (row < 9 && sudoku[row][col] != 0) {
		// Gehe ein Feld weiter
		col++;
		// Wenn wir am Ende einer Zeile angekommen sind
		if (col == 9) {
			// Gehe zur nächsten Zeile
			row++;
			col = 0;
		}
	}
	// Probiere die Werte 1-9 im leeren Feld aus:
	// Durchlaufe die Werte 1-9
	for (int i = 1; i <= 9; i++) {
		// Setze den aktuellen Wert in das aktuelle Feld
		sudoku[row][col] = i;
		// Prüfe ob sich daraus ein gültiges Sudoku ergibt
		if (checkRow(row)
			&& checkCol(col)
			&& checkBlock(row, col)
			&& solveRecursion(row, col)) {
			return true;
			
		}
	}
	// Wenn bis hier kein true zurück gegeben wurde:
	// Wert zurücksetzen und erneut aufrufen lassen.
	sudoku[row][col] = 0;
	return false;
}

void SudokuProblem::solvePreparation() {
	// Algorithmus nach http://www.instructables.com/id/Solve-Sudoku-Without-even-thinking!/&ALLSTEPS
	// Findet eindeutige Lösungen, heißt wenn nur eine Zahl möglich ist
	
	bool preparationFinished = true;
	while (!preparationFinished) {
		preparationFinished = true;
		removeChoices();
		if (findSimpleSolutions()) preparationFinished = false;
		for (int i = 0; i < 9; i++) {
			if (findRowSolutions(i)) preparationFinished = false;
			if (findColSolutions(i)) preparationFinished = false;
			if (findBlockSolutions(i)) preparationFinished = false;
		}
	}
	cout << "Vorbereitung abgeschlossen" << endl;
}

void SudokuProblem::removeChoices() {
	// Löschen der nicht mehr möglichen Zahlen
	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {
			// Wenn eine Zahl im Feld steht und es noch nicht abgearbeitet wurde
			if (sudoku[row][col] != 0 && !finished[row][col]) {
				// Lösche die Möglichkeiten in diesem Feld
				choices[row][col].clear();
				// Gehe die Reihe durch
				for (int x = 0; x < 9; x++) {
					// Wenn in dem Feld noch keine Zahl eingetragen ist
					if (sudoku[row][col] == 0) {
						// Streiche die Zahl aus diesem Feld aus den Möglichkeiten
						choices[row][x].remove(sudoku[row][col]);
					}
				}
				// Gehe die Spalte durch
				for (int y = 0; y < 9; y++) {
					// Wenn in dem Feld noch keine Zahl eingetragen ist
					if (sudoku[row][col] == 0) {
						// Streiche die Zahl aus diesem Feld aus den Möglichkeiten
						choices[y][col].remove(sudoku[row][col]);
					}
				}
				// Gehe den Block durch
				for (int by = 0; by < 3; by++) {
					for (int bx = 0; bx < 3; bx++) {
						int x = row / 3 * 3 + bx;
						int y = col / 3 * 3 + by;
						// Wenn in dem Feld noch keine Zahl eingetragen ist
						if (sudoku[row][col] == 0) {
							// Streiche die Zahl aus diesem Feld aus den Möglichkeiten
							choices[x][y].remove(sudoku[row][col]);
						}
					}
				}
				// Markiere das Feld als bearbeitet
				finished[row][col] = true;
			}
		}
	}
}

bool SudokuProblem::findSimpleSolutions() {
	bool foundSolution = false;
	// Finde die eindeutigen Lösungen
	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {
			// Wenn nur eine Möglichkeit besteht, ist dies die Lösung
			if (choices[row][col].size() == 1) {
				// Setze das Feld auf die verbleibende Möglichkeit
				sudoku[row][col] = choices[row][col].front();
				// Lösche die Möglichkeiten in diesem Feld
				choices[row][col].clear();
				// Wir haben etwas gefunden
				foundSolution = true;
				// Ausgabe der Lösung
				cout << "[" << row << ", " << col << "] = " << sudoku[row][col]
					<< " wurde in der Vorbereitung als Loesung gefunden." << endl;
			}
		}
	}
	return foundSolution;
}

bool SudokuProblem::findRowSolutions(int row) {
	bool foundSolution = false;
	int counter[9];
	struct Vector2 lastSeen[9];
	// Zähle, wie oft eine Möglichkeit in dieser Zeile vorkommt
	for (int col = 0; col < 9; col++) {
		for each(int c in choices[row][col]) {
			counter[c - 1]++;
			// Speichere die letzte Position der gezählten Zahl
			lastSeen[c - 1].x = row;
			lastSeen[c - 1].y = col;
		}
	}
	// Durchlaufe das Counter Array
	for (int c = 0; c < 9; c++) {
		// Wenn eine Möglichkeit nur ein mal vorkam
		if (counter[c] == 1) {
			// Setze die Zahl dort als Lösung, wo sie zuletzt gesehen wurde
			sudoku[lastSeen[c].x][lastSeen[c].y] = c + 1;
			// Lösche die Möglichkeiten in diesem Feld
			choices[lastSeen[c].x][lastSeen[c].y].clear();
			// Wir haben etwas gefunden
			foundSolution = true;
			// Ausgabe der Lösung
			cout << "[" << lastSeen[c].x << ", " << lastSeen[c].y << "] = " << sudoku[lastSeen[c].x][lastSeen[c].y]
				<< " wurde in der Vorbereitung als Loesung gefunden." << endl;
		}
	}
	return foundSolution;
}

bool SudokuProblem::findColSolutions(int col) {
	bool foundSolution = false;
	int counter[9];
	struct Vector2 lastSeen[9];
	// Zähle, wie oft eine Möglichkeit in dieser Spalte vorkommt
	for (int row = 0; row < 9; row++) {
		for each(int c in choices[row][col]) {
			counter[c - 1]++;
			// Speichere die letzte Position der gezählten Zahl
			lastSeen[c - 1].x = row;
			lastSeen[c - 1].y = col;
		}
	}
	// Durchlaufe das Counter Array
	for (int c = 0; c < 9; c++) {
		// Wenn eine Möglichkeit nur ein mal vorkam
		if (counter[c] == 1) {
			// Setze die Zahl dort als Lösung, wo sie zuletzt gesehen wurde
			sudoku[lastSeen[c].x][lastSeen[c].y] = c + 1;
			// Lösche die Möglichkeiten in diesem Feld
			choices[lastSeen[c].x][lastSeen[c].y].clear();
			// Wir haben etwas gefunden
			foundSolution = true;
			// Ausgabe der Lösung
			cout << "[" << lastSeen[c].x << ", " << lastSeen[c].y << "] = " << sudoku[lastSeen[c].x][lastSeen[c].y]
				<< " wurde in der Vorbereitung als Loesung gefunden." << endl;
		}
	}
	return foundSolution;
}

bool SudokuProblem::findBlockSolutions(int block) {
	bool foundSolution = false;
	int counter[9];
	struct Vector2 lastSeen[9];
	// Zähle, wie oft eine Möglichkeit in diesem Block vorkommt
	for (int row = block / 3 * 3; row < block / 3 * 3 + 3; row++) {
		for (int col = block % 3 * 3; col < block % 3 * 3 + 3; col++) {
			for each(int c in choices[row][col]) {
				counter[c - 1]++;
				// Speichere die letzte Position der gezählten Zahl
				lastSeen[c - 1].x = row;
				lastSeen[c - 1].y = col;
			}
		}
	}
	// Durchlaufe das Counter Array
	for (int c = 0; c < 9; c++) {
		// Wenn eine Möglichkeit nur ein mal vorkam
		if (counter[c] == 1) {
			// Setze die Zahl dort als Lösung, wo sie zuletzt gesehen wurde
			sudoku[lastSeen[c].x][lastSeen[c].y] = c + 1;
			// Lösche die Möglichkeiten in diesem Feld
			choices[lastSeen[c].x][lastSeen[c].y].clear();
			// Wir haben etwas gefunden
			foundSolution = true;
			// Ausgabe der Lösung
			cout << "[" << lastSeen[c].x << ", " << lastSeen[c].y << "] = " << sudoku[lastSeen[c].x][lastSeen[c].y]
				<< " wurde in der Vorbereitung als Loesung gefunden." << endl;
		}
	}
	return foundSolution;
}

bool SudokuProblem::checkRow(int row) {
	int counter[9];
	for (int i = 0; i < 9; i++) counter[i] = 0;
	// Durchlaufe die Zeile
	for (int col = 0; col < 9; col++) {
		// Wenn das Feld eine Zahl enthält
		if (sudoku[row][col] != 0) {
			// und der counter für die Zahl nicht 0 ist 
			if (counter[sudoku[row][col]-1] != 0) return false;
			counter[sudoku[row][col] - 1]++;
		}
	}
	return true;
}

bool SudokuProblem::checkCol(int col) {
	int counter[9];
	for (int i = 0; i < 9; i++) counter[i] = 0;
	// Durchlaufe die Spalte
	for (int row = 0; row < 9; row++) {
		// Wenn das Feld eine Zahl enthält
		if (sudoku[row][col] != 0) {
			// und der counter für die Zahl nicht 0 ist 
			if (counter[sudoku[row][col] - 1] != 0) return false;
			counter[sudoku[row][col] - 1]++;
		}
	}
	return true;
}

bool SudokuProblem::checkBlock(int row, int col) {
	int counter[9];
	for (int i = 0; i < 9; i++) counter[i] = 0;
	// Bestimme den Block
	int block = row/3*3 + col/3;
	// Durchlaufe den Block
	for (int row = block / 3 * 3; row < block / 3 * 3 + 3; row++) {
		for (int col = block % 3 * 3; col < block % 3 * 3 + 3; col++) {
			// Wenn das Feld eine Zahl enthält
			if (sudoku[row][col] != 0) {
				// und der counter für die Zahl nicht 0 ist 
				if (counter[sudoku[row][col] - 1] != 0) return false;
				counter[sudoku[row][col] - 1]++;
			}
		}
	}
	return true;
}

bool SudokuProblem::isSolved() {
	for (int row = 0; row < sudoku.size(); row++) {
		for (int col = 0; col < sudoku[row].size(); col++) {
			if (sudoku[row][col] == 0) return false;
		}
	}
	return true;
}

void SudokuProblem::print() {
	// Iteriere durch die Zeilen und Spalten und gebe die Werte aus
	for (int row = 0; row < sudoku.size(); row++) {
		for (int col = 0; col < sudoku[row].size(); col++) {
			// Ausgabe der Werte
			if (sudoku[row][col] == 0) cout << "_";
			else cout << (int)sudoku[row][col];
			// Formatierung: Vertikale Lücken
			if ((col + 1) % 3 == 0) cout << "  ";
			else cout << " ";
		}
		// Formatierung: Horizontale Lücken
		if ((row + 1) % 3 == 0) cout << endl << endl;
		else cout << endl;
	}
}

Mat SudokuProblem::getImage() {
	Mat image = Mat(408,408,CV_8UC3);
	image.setTo(255);	
	for (int x = 0; x < image.size().width; x++) {
		for (int y = 0; y < image.size().height; y++) {
			if (x % 45 == 1)
				line(image, Point(x, 0), Point(x, image.size().height), Scalar(0, 0, 0), 1, 8, 0);
			if (y % 45 == 1)
				line(image, Point(0, y), Point(image.size().width, y), Scalar(0, 0, 0), 1, 8, 0);
			if (x % 135 == 1)
				line(image, Point(x, 0), Point(x, image.size().height), Scalar(0, 0, 0), 3, 8, 0);
			if (y % 135 == 1)
				line(image, Point(0, y), Point(image.size().width, y), Scalar(0, 0, 0), 3, 8, 0);
		}
	}
	// Zeichen des Sudoku Problems in das Image
	int fontFace = FONT_HERSHEY_PLAIN;
	double fontScale = 2;
	int thickness = 2;
	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {
			std::stringstream sstm;
			if (sudoku[row][col] == 0) {
				sstm << " ";
			}
			else {
				sstm << (int)sudoku[row][col];
			}
			string result = sstm.str();
			cv::Point textOrigin(45*col+14, 45*(row+1)-10);
			cv::putText(image, result, textOrigin, fontFace, fontScale, Scalar(0, 0, 0), thickness, 8);
		}
	}
//	namedWindow("Test", CV_WINDOW_AUTOSIZE);
//	imshow("Test", image);
	return image;
}
