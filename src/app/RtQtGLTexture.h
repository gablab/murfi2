/******************************************************************************
 * RtQtGLTexture.h class for wrapping opengl textures for QT
 *
 * Oliver Hinds <ohinds@mit.edu> 2010-10-13
 * 
 * based on an the 'texture' opengl example qt program, 
 * which is Copyright (C) 1992-2008 Trolltech ASA.  
 *
 ****************************************************************************/

#ifndef RTQTGLTEXTURE_H
#define RTQTGLTEXTURE_H

#include<qgl.h>
#include<vnl/vnl_vector.h>

#include<string>
using namespace std;

class RtQtGLTexture : public QGLWidget
{
    Q_OBJECT

public:

    RtQtGLTexture(QWidget* parent, const char* name);
    ~RtQtGLTexture();

	void   SetThreshold(double _threshold) { threshold = _threshold; }
	double GetThreshold()                  { return threshold; }

public slots:

protected:

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    virtual GLuint 	makeObject(const QImage& tex1, const QImage& tex2);

private:

	double threshold; // for stat display

	int    imageW;
	int    imageH;
	GLuint imageTex;
	GLuint posOverlayTex;
	GLuint negOverlayTex;
	GLuint posMaskTex;
	GLuint negMaskTex;

	// holds activation sum over time
	vnl_vector<double> postc;
	vnl_vector<double> negtc;

	// data ids to watch for in setData
	string imageDisplayType;
	string posOverlayID;
	string posOverlayRoiID;
	string negOverlayID;
	string negOverlayRoiID;
	string posActivationSumID;
	string posActivationSumRoiID;
	string negActivationSumID;
	string negActivationSumRoiID;
	string posMaskID;
	string posMaskRoiID;
	string negMaskID;
	string negMaskRoiID;

	// flags
	bool needsRepaint;
	bool newTex;
	bool newPosOverlay;
	bool newNegOverlay;
	bool newPosMask;
	bool newNegMask;
	bool newImageType;

	bool posOverlayOn;
	bool negOverlayOn;
	bool posMaskOn;
	bool negMaskOn;

    QTimer* timer;
};


#endif 
