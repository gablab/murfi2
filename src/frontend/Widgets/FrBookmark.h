// class for storing bookmark info

#ifndef FR_BOOKMARK
#define FR_BOOKMARK

#include "Qt/QString.h"
#include "QtGui/QWidget"

class FrBookmark: public QWidget
{
	Q_OBJECT
public:
	FrBookmark(QWidget *parent = 0);
    
    int GetID();
        
    QString& GetName();
    void SetName(QString& value);

    QString& GetDescription();
    void SetDescription(QString& value);    
	
public slots:
    // Add slots here 

signals:    
    // add signals here
	void Clicked(FrBookmark &bookmark);
    void NameChanged(QString& newName);

protected:
	virtual void mousePressEvent(QMouseEvent *event);

private:
    int      m_ID;
    QString m_name;
    QString m_description;

    static int currentID;
};

#endif