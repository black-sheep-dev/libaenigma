#ifndef TOOLS_H
#define TOOLS_H

#include <QVector>

#include "aenigma.h"

constexpr quint32 MAX_SOLUTION_TRIES{5000000};

namespace Aenigma {

class Tools
{
public:
    Tools() = default;

    // helper functions
    static quint8 dice();
    static void fillBox(quint8 row, quint8 col, QVector<quint8> &board, quint8 (*dice)());
    static quint8 findEmptyCell(QVector<quint8> &board);
    static QVector<quint8> getShuffledNumbers();
    static quint8 index(quint8 row, quint8 column) { return row * 9 + column; }
    static bool isValid(quint8 num, quint8 row, quint8 col, const QVector<quint8> &board);
    static bool isValidBox(quint8 num, quint8 row, quint8 col, const QVector<quint8> &board);
    static bool isValidCol(quint8 num, quint8 col, const QVector<quint8> &board);
    static bool isValidRow(quint8 num, quint8 row, const QVector<quint8> &board);
    static quint8 noteToNumber(Note::Number note);
    static quint8 numberOfSolutions(QVector<quint8> &board, quint32 &tries, quint8 pos = 0, quint8 count = 0);
    static quint16 numberToNote(quint8 number);
    static bool removeElements(QVector<quint8> &board, quint8 n);
    static void shuffleVector(QVector<quint8> &vector);
    static bool solveBoard(QVector<quint8> &board);
    static void swapNumber(quint8 i, quint8 j, QVector<quint8> &vector);

    // main tools
    static void generateBoard(QVector<quint8> &board);
    static void generateNotes(QVector<quint16> &notes, const QVector<quint8> &board);

};

}



#endif // TOOLS_H
