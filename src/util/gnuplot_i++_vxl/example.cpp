#include <unistd.h>
#include "gnuplot_i_vxl.h"

#define SLEEP_LGTH 1
#define NPOINTS    50

int main(int argc, char *argv[])
{
    vector<double> x;
    vector<double> y;
    vector<double> z;
    vnl_vector<double> vx;
    vnl_vector<double> vy;
    vnl_vector<double> vz;
    int i;


    cout << "*** example of gnuplot control through C++ ***" << endl;

    Gnuplot g1 = Gnuplot("lines");

    //
    // Slopes
    // 
    cout << "*** plotting slopes" << endl;
    cout << "y = x" << endl;
    g1.plot_slope(1.0,0.0,"unity slope");
    sleep(SLEEP_LGTH);

    cout << "y = 2*x" << endl;
    g1.plot_slope(2.0,0.0,"y=2x");
    sleep(SLEEP_LGTH);

    cout << "y = -x" << endl;
    g1.plot_slope(-1.0,0.0,"y=-x");
    sleep(SLEEP_LGTH);

    //
    // Equations
    //
    g1.reset_plot();
    cout << endl << endl << "*** various equations" << endl;
    cout << "y = sin(x)" << endl;
    g1.plot_equation("sin(x)","sine");
    sleep(SLEEP_LGTH);

    cout << "y = log(x)" << endl;
    g1.plot_equation("log(x)","logarithm");
    sleep(SLEEP_LGTH);

    cout << "y = sin(x) * cos(2*x)" << endl;
    g1.plot_equation("sin(x)*cos(2*x)","sine product");
    sleep(SLEEP_LGTH);

    //
    // Styles
    //
    g1.reset_plot();
    cout << endl << endl << "*** showing styles" << endl;
    cout << "sine in points" << endl;
    g1.set_style("points");
    g1.plot_equation("sin(x)","sine");
    sleep(SLEEP_LGTH);

    cout << "sine in impulses" << endl;
    g1.set_style("impulses");
    g1.plot_equation("sin(x)","sine");
    sleep(SLEEP_LGTH);

    cout << "sine in steps" << endl;
    g1.set_style("steps");
    g1.plot_equation("sin(x)","sine");
    sleep(SLEEP_LGTH);

    //
    // User defined 1d, 2d and 3d point sets
    //
    cout << endl << endl << "*** user-defined lists of doubles" << endl;
    vx.set_size(NPOINTS);
    for (i = 0; i < NPOINTS; i++) {
        x.push_back((double)i * (double)i);
        vx.put(i,(double)i * (double)i);
    }
    g1.reset_plot();
    g1.set_style("impulses");
    g1.plot_x(x,"user-defined doubles");
    sleep(SLEEP_LGTH);

    g1.reset_plot();
    g1.set_style("impulses");
    g1.plot_x(vx,"user-defined doubles (vnl_vector)");
    sleep(SLEEP_LGTH);


    cout << endl << endl << "*** user-defined lists of points" << endl;
    x.clear();
    vx.clear();
    vx.set_size(NPOINTS);
    vy.set_size(NPOINTS);
    for (i = 0; i < NPOINTS; i++)
      {
        x.push_back((double)i);
        y.push_back((double)i * (double)i);
        vx.put(i,(double)i);
        vy.put(i,(double)i * (double)i);
      }
    g1.reset_plot();
    g1.set_style("points");
    g1.plot_xy(x,y,"user-defined points");
    sleep(SLEEP_LGTH);

    g1.reset_plot();
    g1.set_style("points");
    g1.plot_xy(vx,vy,"user-defined points (vnl_vector)");
    sleep(SLEEP_LGTH);


    vx.clear();
    vy.clear();
    vx.set_size(NPOINTS);
    vy.set_size(NPOINTS);
    vz.set_size(NPOINTS);
    x.clear();
    y.clear();
    for (i = 0; i < NPOINTS; i++)
      {
        x.push_back((double)i);
        y.push_back((double)i * (double)i);
	z.push_back((double)i * (double)i * (double)i);

        vx.put(i,(double)i);
        vy.put(i,(double)i * (double)i);
	vz.put(i,(double)i * (double)i * (double)i);
      }
    g1.reset_plot();
    g1.set_style("points");
    g1.plot_xyz(x,y,z,"user-defined points");
    sleep(SLEEP_LGTH);

    g1.reset_plot();
    g1.set_style("points");
    g1.plot_xyz(vx,vy,vz,"user-defined points (vnl_vector)");
    sleep(SLEEP_LGTH);




    //
    // Multiple output screens
    //
    cout << endl << endl;
    cout << "*** multiple output windows" << endl;
    g1.reset_plot();
    g1.set_style("lines");

    Gnuplot g2 = Gnuplot("lines");
    Gnuplot g3 = Gnuplot("lines");

    cout << "window 1: sin(x)" << endl;
    g1.plot_equation("sin(x)","sin(x)");
    sleep(SLEEP_LGTH);

    cout << "window 2: x*sin(x)" << endl;
    g2.plot_equation("x*sin(x)","x*sin(x)");
    sleep(SLEEP_LGTH);

    cout << "window 3: log(x)/x" << endl;
    g3.plot_equation("log(x)/x","log(x)/x");
    sleep(SLEEP_LGTH);

    //
    // Using the GnuplotException class
    //
    try {
        Gnuplot g4 = Gnuplot("lines");
        cout << "window 4: sin(x)/x" << endl;
        g4.plot_equation("sin(x)/x","sin(x)/x");
        sleep(SLEEP_LGTH);

    } catch (GnuplotException ge) {
        cout << ge.what() << endl;
    }

	//
	// Plot an image
	//
	g1.reset_plot();
	int iWidth  = 255;
	int iHeight = 255;
	g1.set_xrange(0,iWidth);
	g1.set_yrange(0,iHeight);
	g1.set_cbrange(0,255);
	g1.cmd("set palette gray");
	unsigned char ucPicBuf[iWidth*iHeight];
	// generate a greyscale image
	for(int iIndex = 0; iIndex < iHeight*iWidth; iIndex++)
	{
			ucPicBuf[iIndex] = iIndex%255;
	}
	g1.plot_image(ucPicBuf,iWidth,iHeight,"greyscale");
    sleep(SLEEP_LGTH+10);	
	

    cout << endl << "*** end of gnuplot example" << endl;

    return 0;
}
