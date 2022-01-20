#include "tools.h"

#include <QtMath>

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
#include <QRandomGenerator>
#endif

using namespace Aenigma;

quint8 Tools::dice()
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    return QRandomGenerator::global()->bounded(1, boxSize + 1);
#else
    return qrand() % boxSize + 1;
#endif
}

void Tools::fillBox(quint8 row, quint8 col, QVector<quint8> &board, quint8 (*dice)())
{
    for (quint8 i = row * 3; i < (row + 1) * 3; ++i) {
        for (quint8 j = col * 3; j < (col + 1) * 3; ++j) {
            bool filled = false;
            while (!filled) {
                quint8 num = dice();

                if (isValidBox(num, i, j, board)) {
                    board[index(i, j)] = num;
                    filled = true;
                }
            }
        }
    }
}

quint8 Tools::findEmptyCell(QVector<quint8> &board)
{
    qint8 pos = -1;

    for (qint8 i = 0; i < gridSize && pos == -1; ++i) {
        if (board[i] == 0) {
            pos = i;
        }
    }

    return pos;
}

QVector<quint8> Tools::getShuffledNumbers()
{
    QVector<quint8> numbers;
    numbers.resize(boxSize);

    for (quint8 i = 0; i < boxSize; ++i) {
        numbers[i] = i + 1;
    }

    shuffleVector(numbers);

    return numbers;
}

bool Tools::isValid(quint8 num, quint8 row, quint8 col, const QVector<quint8> &board)
{
    return isValidBox(num, row, col, board) && isValidCol(num, col, board) && isValidRow(num, row, board);
}

bool Tools::isValidBox(quint8 num, quint8 row, quint8 col, const QVector<quint8> &board)
{
    const quint8 R = row - row % 3;
    const quint8 C = col - col % 3;

    for (quint8 i = 0; i < 3; ++i) {
        for (quint8 j = 0; j < 3; ++j) {
            if (num == board[index(R + i, C + j)]) return false;
        }
    }

    return true;
}

bool Tools::isValidCol(quint8 num, quint8 col, const QVector<quint8> &board)
{
    for (quint8 i = 0; i < boxSize; ++i) {
        if (num == board[index(i, col)]) {
            return false;
        }
    }

    return true;
}

bool Tools::isValidRow(quint8 num, quint8 row, const QVector<quint8> &board)
{
    for (quint8 i = 0; i < boxSize; ++i) {
        if (num == board[index(row, i)]) {
            return false;
        }
    }

    return true;
}

quint8 Tools::noteToNumber(Note::Number note)
{
    switch (note) {
    case Note::One:
        return 1;
    case Note::Two:
        return 2;
    case Note::Three:
        return 3;
    case Note::Four:
        return 4;
    case Note::Five:
        return 5;
    case Note::Six:
        return 6;
    case Note::Seven:
        return 7;
    case Note::Eight:
        return 8;
    case Note::Nine:
        return 9;
    default:
        return 0;
    }
}

quint8 Tools::numberOfSolutions(QVector<quint8> &board, quint32 &tries, quint8 pos, quint8 count)
{
    tries++;

    if (tries > MAX_SOLUTION_TRIES) {
        return 0;
    }

    if (pos == gridSize) {
        return 1 + count;
    }

    if (board[pos] != 0) {
        return numberOfSolutions(board, tries, pos + 1, count);
    } else {
        for (quint8 val = 1; val <= boxSize && count < 2; ++val) {
            if (isValid(val, pos / boxSize, pos % boxSize, board)) {
                board[pos] = val;
                count = numberOfSolutions(board, tries, pos + 1, count);
            }
        }
        board[pos] = 0;
        return count;
    }
}

quint16 Tools::numberToNote(quint8 number)
{
    switch (number) {
    case 1:
        return Note::One;
    case 2:
        return Note::Two;
    case 3:
        return Note::Three;
    case 4:
        return Note::Four;
    case 5:
        return Note::Five;
    case 6:
        return Note::Six;
    case 7:
        return Note::Seven;
    case 8:
        return Note::Eight;
    case 9:
        return Note::Nine;
    default:
        return Note::None;
    }
}

bool Tools::removeElements(QVector<quint8> &board, quint8 n)
{
    quint8 pos{0};
    quint8 value{0};

    quint32 tries{0};

    for (quint8 i = 0; i < n;) {
        pos = index(dice() - 1, dice() - 1);
        value = board[pos];
        if (value != 0) {
            board[pos] = 0;
            const quint8 solutions = numberOfSolutions(board, tries);
            if (solutions > 1) {
                board[pos] = value;
                continue;
            } else if (solutions == 0) {
                return false;
            }
            ++i;
        }
    }

    return true;
}

void Tools::shuffleVector(QVector<quint8> &vector)
{
    quint8 cnt{0};
    while (cnt < vector.size()) {
        quint8 i = dice() - 1;
        quint8 j = dice() - 1;
        if (i != j) {
            swapNumber(i, j, vector);
            cnt++;
        }
    }
}

bool Tools::solveBoard(QVector<quint8> &board)
{
    QVector<quint8> numbers(getShuffledNumbers());

    const qint8 pos = findEmptyCell(board);

    if (pos == -1) {
        return true;
    }

    for (quint8 num = 0; num < boxSize; num++) {
        const unsigned short number = numbers[num];
        if (isValid(number, pos / boxSize, pos % boxSize, board)) {
            board[pos] = number;
            if (solveBoard(board)) {
                return true;
            } else {
                board[pos] = 0;
            }
        }
    }
    return false;
}

void Tools::swapNumber(quint8 i, quint8 j, QVector<quint8> &vector)
{
    quint8 temp = vector[i];
    vector[i] = vector[j];
    vector[j] = temp;
}

void Tools::generateBoard(QVector<quint8> &board)
{
    board.fill(0);

    for (quint8 i = 0; i < 3; ++i) {
        fillBox(i, i, board, dice);
    }

    solveBoard(board);
}

void Aenigma::Tools::generateNotes(QVector<quint16> &notes, const QVector<quint8> &board)
{
    notes.fill(Note::None);

    for (int i = 0; i < gridSize; ++i) {
        if (board[i] != 0) continue;
        const quint8 row = qFloor(i / rowSize);
        const quint8 col = i - row * rowSize;

        for (int n = 1; n <= boxSize; ++n) {
            if (!isValid(n, row, col, board)) continue;
            notes[i] |= numberToNote(n);
        }
    }
}
