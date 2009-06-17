/******************************************************************************
 * template class definition for a Document Reader
 *****************************************************************************/
template <class T>
T* FrDocumentReader::GetSliceDataXY(RtDataImage<T>* img, int slice){    
    T* result = 0;
    
    // NOTE: assume data are unmosaic
    if(0 <= slice && slice < img->getDim(2)){
        // Init data
        int dataSize = img->getDim(0) * img->getDim(1);
        result = new T[dataSize];
        T* dstPtr = result;
        T* srcPtr = img->data + (slice * dataSize);
        
        memcpy(dstPtr, srcPtr, dataSize * sizeof(T));
    }

    return result;
}

template <class T>
T* FrDocumentReader::GetSliceDataYZ(RtDataImage<T>* img, int slice){
    T* result = 0;

    // NOTE: assume data are unmosaic
    if(0 <= slice && slice < img->getDim(0)){
        // Init data
        unsigned int dataSize = img->getDim(1) * img->getDim(2);
        result = new T[dataSize];
        T* dstPtr = result;
        T* srcPtr = img->data + m_Slice;

        int stride = img->getDim(0);
        for(int i=0; i < dataSize; ++i){
            // copy vertical line from each slice
            (*dstPtr) = (*srcPtr);
            srcPtr += stride;
            ++dstPtr;
        }
    }

    return result;
}

template <class T>
T* FrDocumentReader::GetSliceDataXZ(RtDataImage<T>* img, int slice){
    T* result = 0;

    // NOTE: assume data are unmosaic
    if(0 <= slice && slice < img->getDim(1)){
        // Init data
        int lineSize = img->getDim(0);
        result = new T[img->getDim(0) * img->getDim(2)];
        T* dstPtr = result;
        T* srcPtr = img->data + (slice * lineSize);
        
        int lineCnt = img->getDim(2);
        int lineStride = img->getDim(0) * img->getDim(1) - lineSize;
        for(int i=0; i < lineCnt; ++i){
            // copy horizontal line from each slice
            for(int j=0; j < lineSize; ++j){
                (*dstPtr) = (*srcPtr);
                ++dstPtr; ++srcPtr;
            }
            srcPtr += lineStride;
        }
    }

    return result;
}
