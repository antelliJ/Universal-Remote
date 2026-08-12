#include <vector>

#include "IRProfilesList.hpp"
#include "BTProfilesList.hpp"



// concat the bluetooth and ir profiles
std::vector<DeviceProfile*> Profiles =  {};
void addProfiles(std::vector<DeviceProfile*> profiles) {
    
    Profiles.insert(Profiles.end(), profiles.begin(), profiles.end());
}

void allProfileSetup(){
    addProfiles(IRProfiles);
    addProfiles(BTProfiles);
    
}
