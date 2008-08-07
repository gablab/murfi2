#include <Qt/qapplication.h>

class FrApplication : public QApplication {
Q_OBJECT
public:
	FrApplication(int argc, char **argv, bool useInterface = true);
	~FrApplication();

protected:


private:


};