#ifndef GENERATOR_H
#define GENERATOR_H

#include <QRunnable>
#include <QObject>

#include "aenigma.h"
#include "tools.h"

#include "libaenigma_global.h"

namespace Aenigma {

class LIBAENIGMA_EXPORT Generator : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit Generator(quint8 difficulty = Difficulty::Medium, QObject *parent = nullptr);

signals:
    void failed();
    void finished(QVector<quint8> puzzle, QVector<quint8> solution, QVector<quint16> notes);

private:
    quint8 m_difficulty{Difficulty::Medium};
    QVector<quint8> m_difficulties{25,35,45,55};

    // QRunnable interface
public:
    void run() override;
};

}



#endif // GENERATOR_H
