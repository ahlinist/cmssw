#ifndef TrackerMap_Display_Plugins_h
# define TrackerMap_Display_Plugins_h

//<<<<<< INCLUDES                                                       >>>>>>
# include "VisMonitoring/VisDQMBase/interface/VisDQMDisplayPlugin.h"
# include "CommonTools/TrackerMap/interface/VisTrackerMapBox.h"
//# include "Ig_Modules/IgStudio/interface/IgDocumentData.h"


//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class TrackerMapDisplayPlugins : public VisDQMDisplayPlugin
{
public:
    static const char * catalogLabel (void) 
	{
	    return "SiStrip TrackerMap";	    
	}	
    
    TrackerMapDisplayPlugins (IgState *state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual bool applies (DisplayData *data);
    virtual std::string preDraw (DisplayData *data);
    virtual void postDraw (DisplayData *data);
 
 private:

    VisTrackerMapBox *tkMapBox;
//    IgDocumentData * igDocumentData;

    int updateCounter;
};

#endif // TrackerMap_Display_Plugins_h
