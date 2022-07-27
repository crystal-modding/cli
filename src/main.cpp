#define CLI_VERSION "v0.1-alpha"

#include <iostream>
#include <algorithm>
#include <Windows.h>
#include <nfd.hpp>
#include <filesystem>
#include <string>
#include "color.hpp"
#include "SimpleIni.h"

namespace fs = std::filesystem;

CSimpleIniA ini;


std::string getModloader(fs::path outPath)
{
    if (fs::exists(outPath / "hackpro.dll"))
    {
        return "HACKPRO";
    } else if (fs::exists(outPath / "ToastedMarshmellow.dll")) {
        return "GDHM";
    } else if (fs::exists(outPath / "adaf-dll")) {
        return "PROXYDLLLOADER";
    } else {
        return "NONE";
    }
}

fs::path getExtensionsPath(std::string modloader)
{
    if (modloader == "HACKPRO")
    {
        return fs::path("extensions");
    } else if (modloader == "GDHM")
    {
        return fs::path(".GDHM\\dll");
    } else if (modloader == "PROXYDLLLOADER")
    {
        return fs::path("adaf-dll");
    } else {
        return fs::path(".");
    }
}

void createProject()
{

}

void launchGD()
{
    system("start steam://rungameid/322170");
}

void installDLL(char* filename)
{
    ini.LoadFile("config.ini");
    
    const char* path = ini.GetValue("crystal", "path");
    const char* modloader = ini.GetValue("crystal", "modloader");

    auto extPath = getExtensionsPath(std::string(modloader));

    
    if (!path)
    {
        std::cout << dye::light_red("ERROR: GD path not found. Run ""crystal setup""") << std::endl;
        return;
    }
    if (!fs::exists(filename))
    {
        std::cout << dye::light_red("ERROR: File not found") << std::endl;
        return;
    }
    std::cout << dye::light_blue("Filename: \t") << (filename) << std::endl;
    std::cout << dye::light_blue("Path: \t\t") << (path / extPath) << std::endl;
    std::cout << dye::light_blue("Modloader: \t") << (modloader) << std::endl;
    std::cout << "\nDo you want to continue? [Y/N] : " << hue::light_blue;
    char answer;
    std::cin >> answer;
    std::cout << hue::reset;
    if (answer == 'Y' || answer == 'y')
    {
        try
        {
            fs::copy(filename, path / extPath, fs::copy_options::overwrite_existing);
        }
        catch (...)
        {
            std::cout << dye::light_red("ERROR: Unknown error") << std::endl;
            return;
        }
        
        std::cout << dye::light_green("Mod has been successfully installed!") << std::endl;
    } else {
        std::cout << dye::light_red("Aborted.") << std::endl;
        return;
    }
}

void setup()
{
    nfdchar_t *outPath;

    auto result = NFD_PickFolder(&outPath, "C:\\Program Files (x86)");
    if (result == NFD_OKAY)
    {
        std::string ml = getModloader(fs::path(outPath));
        ini.SetValue("crystal", "path", outPath);
        ini.SetValue("crystal", "modloader", ml.c_str());
        ini.SaveFile("config.ini");
        std::cout << dye::light_green("Configuring successfully finished!") << std::endl;
    } else if (result == NFD_CANCEL)
    {
        std::cout << dye::light_red("ERROR: User pressed cancel") << std::endl;
    }
    
}
char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

int main(int argc, char * argv[])
{

    NFD_Init();
    

    if(cmdOptionExists(argv, argv+argc, "create"))
    {
        createProject();
    } else if (cmdOptionExists(argv, argv+argc, "launch"))
    {
        launchGD();
    } else if (cmdOptionExists(argv, argv+argc, "setup"))
    {
        setup();
    } else if (cmdOptionExists(argv, argv+argc, "install"))
    {
        auto fn = getCmdOption(argv, argv+argc, "install");
        installDLL(fn);
    } else {
        std::cout << dye::green("crystal-cli " ) << CLI_VERSION << std::endl;
        
        std::cout << dye::light_blue("\nUSAGE: ") << std::endl;
        std::cout << "\tcrystal <SUBCOMMAND>" << std::endl;
        
        std::cout << dye::light_blue("\nSUBCOMMANDS: ") << std::endl;
        std::cout << "\tcreate <ProjectName>" << dye::light_green("\tCreate project with provided name") << std::endl;
        std::cout << "\tsetup" << dye::light_green("\t\t\tSet the Geometry Dash installation folder") << std::endl;
        std::cout << "\tlaunch" << dye::light_green("\t\t\tLaunch Geometry Dash") << std::endl;
        std::cout << "\tinstall <Filename>" << dye::light_green("\tInstall DLL file as mod") << std::endl;
    
    }
    

    return 0;
}