#include "generator.h"

#include <QDateTime>

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
#include <QRandomGenerator>
#endif

using namespace Aenigma;

Generator::Generator(Difficulty::Level difficulty, QObject *parent) :
    QObject(parent),
    m_difficulty(difficulty)
{

}

void Generator::run()
{
    // seeding random number generator
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    QRandomGenerator::global()->seed(QDateTime::currentMSecsSinceEpoch());
#else
    qsrand(QDateTime::currentMSecsSinceEpoch());
#endif

    // generate board
    QVector<quint8> solution(gridSize, 0);
    Tools::generateBoard(solution);

    // generate puzzle
    QVector<quint8> puzzle(solution);
    if (!Tools::removeElements(puzzle, m_difficulties[m_difficulty])) {
        emit failed();
        return;
    }

    // generate notes
    QVector<quint16> notes(gridSize, Note::None);
    Tools::generateNotes(notes, puzzle);

    // emit finished
    emit finished(puzzle, solution, notes);
}
