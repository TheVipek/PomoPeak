function Component()
{

}
Component.prototype.createOperations = function()
{
    try
    {
        component.createOperations();

        if (installer.isInstaller())
        {
            if(installer.value("os") == "win")
            {
                //desktop shortcut
                component.addOperation("CreateShortcut",
                                       "@TargetDir@/win64/PomoPeak.exe",
                                       "@DesktopDir@/PomoPeak.lnk",
                                       "workingDirectory=@TargetDir@/win64",
                                       "iconPath=@TargetDir@/win64/PomoPeak.exe", "iconId=0");
                //start menu shortcut
                component.addOperation("CreateShortcut",
                                       "@TargetDir@/win64/PomoPeak.exe",
                                       "@StartMenuDir@/PomoPeak.lnk",
                                       "workingDirectory=@TargetDir@/win64",
                                       "iconPath=@TargetDir@/win64/PomoPeak.exe", "iconId=0")
            }
        }
    }
    catch(e)
    {
        print(e);
    }


}
