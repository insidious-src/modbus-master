#ifndef MAIN_H
#define MAIN_H

#include <QThread>
#include <QRunnable>

//******************************************************************//
class cMainThread : public QRunnable {
   // Q_OBJECT
private:
    void run();
};
//******************************************************************//

#endif // MAIN_H
