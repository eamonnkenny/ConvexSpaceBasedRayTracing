
#include "configuration.hh"

string CProperties::baseDirectory;
string CProperties::btsDirectory;
string CProperties::diagramDirectory;
string CProperties::buildingDirectory;
string CProperties::windowsDirectory;
string CProperties::linuxDirectory;
string CProperties::matlabDirectory;
string CProperties::matlabBuildingDirectory;
string CProperties::matlabRayPathsDirectory;
string CProperties::matlabCoverageDirectory;
string CProperties::matlabThesisDirectory;
string CProperties::measurementDirectory;
string CProperties::matlabOptimisationDirectory;
 
void CProperties::read( const string& inputFileName )
{
    boost::property_tree::ptree pt;
    boost::filesystem::path  basePath;
    basePath = boost::filesystem::current_path();

    std::cerr << "Working directory is defined as: " << basePath.string() << std::endl;

    // Read the stringstream into a Boost property tree, pt
    read_xml( inputFileName, pt );

    
    baseDirectory     = basePath.string() + "/../" + pt.get("directories.baseDirectory", "0");
    btsDirectory      = basePath.string() + "/../" + pt.get("directories.transceivers", "0");
    diagramDirectory  = basePath.string() + "/../" + pt.get("directories.diagrams", "0");
    buildingDirectory = basePath.string() + "/../" + pt.get("directories.buildings", "0");
    windowsDirectory  = basePath.string() + "/../" + pt.get("directories.windowsExecutables", "0");
    linuxDirectory    = basePath.string() + "/../" + pt.get("directories.linuxExecutables", "0");
    matlabDirectory   = basePath.string() + "/../" + pt.get("directories.matlabFiles", "0");

    matlabBuildingDirectory = basePath.string() + "/../" + pt.get("directories.matlabBuildings", "0");
    matlabCoverageDirectory = basePath.string() + "/../" + pt.get("directories.matlabCoverageMaps", "0");
    matlabThesisDirectory   = basePath.string() + "/../" + pt.get("directories.matlabThesisResults", "0");
    matlabRayPathsDirectory = basePath.string() + "/../" + pt.get("directories.matlabRayPaths", "0");
    measurementDirectory    = basePath.string() + "/../" + pt.get("directories.measurementResults", "0");
    matlabOptimisationDirectory = basePath.string() + "/../" + pt.get("directories.matlabOptimisation", "0");
}
