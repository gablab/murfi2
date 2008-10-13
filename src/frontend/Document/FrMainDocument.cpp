#include "FrMainDocument.h"
#include "FrTBCFilter.h"
#include "FrTabSettingsDocObj.h"

FrMainDocument::FrMainDocument() 
: FrDocument(){
}

FrMainDocument::~FrMainDocument(){
}

void FrMainDocument::GetAllImages(std::vector<FrDocumentObj*>& images){
    images.clear();
    GetObjectsByType(images, FrDocumentObj::ImageObject);
}

FrTabSettingsDocObj* FrMainDocument::GetCurrentTabSettings(){
    std::vector<FrDocumentObj*>& objects = this->GetObjects();
        
    FrTabSettingsDocObj* result = 0L;
    if(objects.size() > 0){
        std::vector<FrDocumentObj*>::iterator it, itEnd(objects.end());
        for(it = objects.begin(); it != itEnd; ++it){
            // Check for type 
            if((*it)->GetType() != FrDocumentObj::TabSettings) continue;
            
            // returns first current
            FrTabSettingsDocObj* ts = (FrTabSettingsDocObj*)(*it);
            if(ts->GetIsCurrent()){
                result = ts;
                break;
            }
        }
    }
    return result;
}

// add/remove layer to/from all tabs
void FrMainDocument::SyncronyzeLayers(LayerSettings &ls, int mode){
	// TODO: not finished, need to implement removal
	std::vector<FrDocumentObj*>& objects = this->GetObjects();
	std::vector<FrDocumentObj*>::iterator it, itEnd(objects.end());

	ls.Visible = true;		// test

	for(it = objects.begin(); it != itEnd; ++it){
		// Check for type 
		if((*it)->GetType() != FrDocumentObj::TabSettings) continue;
        
		// returns first current
		FrTabSettingsDocObj* ts = (FrTabSettingsDocObj*)(*it);
		if(!ts->GetIsCurrent())
			if (mode == 1)
				ts->AddLayer(ls);
			else if (mode == 2)
				ts->RemoveLayer(ls);
	}
}