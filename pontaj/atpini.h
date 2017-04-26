#ifndef ATPINI_H
#define ATPINI_H

#include <QString>
#include <QSettings>

class atpIni :QSettings {

	private:
		QSettings *mySettings;

	public:
        atpIni(QObject *parent = 0);

		QString get_dbPath();
		void set_dbPath(QString value);

        QString get_dbType();
		void set_dbType(QString value);

        int get_infoDelay();
        void set_infoDelay(int value);

        QString get_jobAddres();
        void set_jobAddres(QString value);

        QString get_jobPostcode();
        void set_jobPostcode(QString value);

        QString get_jobReference();
        void set_jobReference(QString value);

        QString get_siteName();
        void set_siteName(QString value);

        QString get_dateStartJob();
        void set_dateStartJob(QString value);

        QString get_dateEndJob();
        void set_dateEndJob(QString value);
};

#endif // ATPINI_H
