#ifndef FR_APPLICATION
#define FR_APPLICATION

// Froward declarations
class FrDocument;

// includes
#include <Qt/qapplication.h>

class FrApplication : public QApplication {
Q_OBJECT
public:
	FrApplication(int argc, char **argv, bool useInterface = true);
	~FrApplication();

protected:


private:
    FrDocument* m_doc;

};

#endif