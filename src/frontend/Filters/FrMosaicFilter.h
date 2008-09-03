#ifndef FR_MOSAIC_FILTER
#define FR_MOSAIC_FILTER

// Forward declarations
class vtkImageData;

class FrMosaicFilter{
public:
    static FrMosaicFilter* New();
	void SetInput(vtkImageData* data);
	void SetOutputDimensions(int w, int h, int num);
	vtkImageData* GetOutput();

private:
    FrMosaicFilter();
	int GetNewId(int slice, int x, int y);

private:
	vtkImageData* m_data;
	int m_newDims[3];
	int m_oldDims[3];

};







#endif