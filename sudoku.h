#ifndef SUDOKU_H
#define SUDOKU_H

#include <QObject>

#include <QDateTime>
#include <QList>
#include <QTimer>
#include <QVariant>
#include <QVector>

#include "aenigma.h"
#include "undostep.h"

#include "generator.h"

#include "libaenigma_global.h"

constexpr quint64 AENIGMA_GAME_DATA_MAGIC = 0x41454e49474d41;
constexpr quint16 AENIGMA_GAME_DATA_VERSION = 4;

namespace Aenigma {

class LIBAENIGMA_EXPORT Sudoku : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool autoCleanupNotes READ autoCleanupNotes WRITE setAutoCleanupNotes NOTIFY autoCleanupNotesChanged)
    Q_PROPERTY(bool autoNotes READ autoNotes WRITE setAutoNotes NOTIFY autoNotesChanged)
    Q_PROPERTY(quint16 currentUndoId READ currentUndoId WRITE setCurrentUndoId NOTIFY currentUndoIdChanged)
    Q_PROPERTY(quint8 difficulty READ difficulty WRITE setDifficulty NOTIFY difficultyChanged)
    Q_PROPERTY(qint64 elapsedTime READ elapsedTime WRITE setElapsedTime NOTIFY elapsedTimeChanged)
    Q_PROPERTY(quint16 hintsCount READ hintsCount WRITE setHintsCount NOTIFY hintsCountChanged)
    Q_PROPERTY(QDateTime startTime READ startTime WRITE setStartTime NOTIFY startTimeChanged)
    Q_PROPERTY(quint8 gameState READ gameState NOTIFY gameStateChanged)
    Q_PROPERTY(quint16 stepsCount READ stepsCount WRITE setStepsCount NOTIFY stepsCountChanged)
    Q_PROPERTY(quint16 undoStepCount READ undoStepCount NOTIFY undoStepCountChanged)
    Q_PROPERTY(quint8 unsolvedCellCount READ unsolvedCellCount NOTIFY unsolvedCellCountChanged)
    Q_PROPERTY(QString uuid READ uuid WRITE setUuid NOTIFY uuidChanged)

public:    
    explicit Sudoku(QObject *parent = nullptr);

    Q_INVOKABLE QVariant data(quint8 row, quint8 column, quint8 role) const;
    Q_INVOKABLE bool setData(quint8 row, quint8 column, quint8 role, const QVariant &data, bool undo = false, quint16 undoId = 0);

    Q_INVOKABLE quint8 cellCount() const;
    Q_INVOKABLE bool isInArea(quint8 row, quint8 column, quint8 number) const;
    Q_INVOKABLE quint8 noteToNumber(Note::Number note) const;
    Q_INVOKABLE quint16 numberToNote(quint8 number) const;

    // save & load game state data
    Q_INVOKABLE bool fromBase64(const QString &data);
    Q_INVOKABLE QString toBase64() const;

    // properties
    bool autoCleanupNotes() const;
    void setAutoCleanupNotes(bool cleanup);

    bool autoNotes() const;
    void setAutoNotes(bool enabled);

    quint16 currentUndoId() const;
    void setCurrentUndoId(quint16 id);

    quint8 difficulty() const;
    void setDifficulty(quint8 difficulty);

    qint64 elapsedTime() const;
    void setElapsedTime(qint64 msec);

    quint8 gameState() const;

    quint16 hintsCount() const;
    void setHintsCount(quint16 count);

    const QDateTime &startTime() const;
    void setStartTime(const QDateTime &time);

    quint16 stepsCount() const;
    void setStepsCount(quint16 count);

    quint16 undoStepCount() const;

    quint8 unsolvedCellCount() const;   

    const QString &uuid() const;
    void setUuid(const QString &uuid);

signals:
    void dataChanged(quint8 row, quint8 column, quint8 role, const QVariant &data);
    void generatorFailed();
    void numberFinished(quint8 number, bool finished = true);

    // properties
    void autoCleanupNotesChanged();
    void autoNotesChanged();
    void currentUndoIdChanged();
    void difficultyChanged();
    void elapsedTimeChanged();
    void gameStateChanged();
    void hintsCountChanged();
    void startTimeChanged();
    void stepsCountChanged();
    void undoStepCountChanged();
    void unsolvedCellCountChanged(); 
    void uuidChanged();

public slots:
    void incrementHintsCount();
    void incrementStepsCount();
    void generate();
    void reset();
    void revertToLastCorrectState();
    void start();
    void stop();
    void toogleNote(quint8 row, quint8 column, quint16 note);
    void undo();

private slots:
    void onGeneratorFailed();
    void onGeneratorFinished(const QVector<quint8>& puzzle, const QVector<quint8> &solution, const QVector<quint16> &notes);

private:
    void checkIfFinished();
    void cleanupNotes(quint8 number);
    void incrementUndoId();

    QVector<quint8> m_game{QVector<quint8>(gridSize, 0)};
    quint16 m_lastCorrectUndoId{0};
    QVector<quint16> m_notes{QVector<quint16>(gridSize, 0)};
    QVector<quint16> m_notesGenerated{QVector<quint16>(gridSize, 0)};
    QVector<quint8> m_puzzle{QVector<quint8>(gridSize, 0)};
    QVector<quint8> m_solution{QVector<quint8>(gridSize, 0)};
    QTimer *m_timer{new QTimer(this)};
    QList<UndoStep> m_undoQueue;

    // properties
    bool m_autoCleanupNotes{false};
    bool m_autoNotes{false};
    quint16 m_currentUndoId{0};
    quint8 m_difficulty{Difficulty::Easy};
    qint64 m_elapsedTime{0};
    quint8 m_gameState{GameState::Empty};
    quint16 m_hintsCount{0};
    QDateTime m_startTime;
    quint16 m_stepsCount{0};  
    quint8 m_unsolvedCellCount{0};
    QString m_uuid;
};

}

#endif // SUDOKU_H
