using System.Linq;
using System.Xml.Linq;

namespace cc
{
    internal class Configuration
    {
        private static XElement Debugger()
        {
            var eventLog = new XElement("EventLog");
            eventLog.Add(new XElement("Enable", "No"));
            eventLog.Add(new XElement("Thread", "Yes"));
            eventLog.Add(new XElement("Monitor", "Yes"));
            eventLog.Add(new XElement("Semaphore", "Yes"));
            eventLog.Add(new XElement("MessageQueue", "Yes"));
            eventLog.Add(new XElement("Event", "Yes"));
            eventLog.Add(new XElement("Monitor", "Yes"));
            eventLog.Add(new XElement("User", "Yes"));

            var debugger = new XElement("Debugger");
            //debugger.Add(new XElement("ProgramArguments", new XElement("ProgArg", "-l")));
            debugger.Add(new XElement("JavaBootSource", ""));
            debugger.Add(new XElement("RamDisk", "No"));
            debugger.Add(new XElement("RamDiskSize", "512"));
            debugger.Add(new XElement("RamDiskFileSystem", "2"));
            debugger.Add(new XElement("EnableDebugMsg", "Yes"));
            debugger.Add(new XElement("EnableJavaDebug", "No"));
            debugger.Add(new XElement("EnableJavaPrelinkDebug", "No"));
            debugger.Add(new XElement("AdditionalOptions"));
            debugger.Add(eventLog);
            return debugger;
        }

        private static XElement BootFiles()
        {
            var bootFiles = new XElement("BootFiles");
            bootFiles.Add(new XElement("Microprogram", "~\\bin\\im3910m210.32.mp"));
            bootFiles.Add(new XElement("Testmicroprogtam", "~\\bin\\Tmpgm.mp"));
            bootFiles.Add(new XElement("GPxProgram"));
            bootFiles.Add(new XElement("LinkOutputGPxProgram", "Yes"));
            return bootFiles;
        }

        private static XElement CCompiler(string objectDir, string[] preprocessorDefinitions, string[] includeDirs)
        {
            var cCompiler = new XElement("CCompiler");
            cCompiler.Add(new XElement("OutputDir", objectDir));
            cCompiler.Add(new XElement("DebugInfo", "Yes"));
            cCompiler.Add(new XElement("AsmListing", "No"));
            cCompiler.Add(new XElement("SuppressWarnings", "Yes"));
            cCompiler.Add(new XElement("Options", "")); //-uisaj
            cCompiler.Add(new XElement("AdditionalOptions", "-O"));
            cCompiler.Add(new XElement("PreprocessorDefinitions", preprocessorDefinitions.Select(n =>
            {
                var define = new XElement("Define");
                define.SetAttributeValue("Value", n);
                return define;
            })));
            cCompiler.Add(new XElement("IncludeDirs", includeDirs.Select(n =>
            {
                var define = new XElement("File");
                define.SetAttributeValue("Path", n);
                return define;
            })));
            cCompiler.Add(new XElement("IgnorDevIncludePath", "No"));
            cCompiler.Add(new XElement("C99", "Yes"));
            cCompiler.Add(new XElement("Align", "No"));
            return cCompiler;
        }

        private static XElement Linker(string name)
        {
            var inputLibraries = new[]
            {
                //"rubus.lib",
                "libc.lib",
                //"ish.lib",
                //"effs.lib",
                //"tcpip.lib",
                //"zlib.lib",
                //"cldc.lib",
                //"crypt.lib",
                "im3910m210.lib"
            };

            var linker = new XElement("Linker");
            linker.Add(new XElement("LinkOutput", name + ".gpx"));
            linker.Add(new XElement("StartupFile", "~\\lib\\c0start.obj"));
            linker.Add(new XElement("InputLibraries", inputLibraries.Select(n =>
            {
                var library = new XElement("Library");
                library.SetAttributeValue("Name", n);
                return library;
            })));
            linker.Add(new XElement("Options"));
            linker.Add(new XElement("IgnoreDefaultLibPath", "No"));
            return linker;
        }

        private static XElement Library()
        {
            return new XElement("Library", new XElement("LibOutput"));
        }

        private static XElement Java()
        {
            var classPath = new XElement("Path");
            classPath.SetAttributeValue("Value", ".");

            var bootClassPath = new XElement("Path");
            bootClassPath.SetAttributeValue("Value", "~\\java\\classes");

            var java = new XElement("Java");
            java.Add(new XElement("OutputDir", ""));
            java.Add(new XElement("Options", ""));
            //java.Add(new XElement("ClassPath", classPath));
            //java.Add(new XElement("BootClassPath", bootClassPath));
            java.Add(new XElement("DebugInfo", "Yes"));
            java.Add(new XElement("Verbose", "No"));
            java.Add(new XElement("Target", ""));
            java.Add(new XElement("Preverify", "Yes"));
            java.Add(new XElement("PreverifyOptions", ""));
            return java;
        }

        private static XElement Archive()
        {
            var archive = new XElement("Archive");
            archive.Add(new XElement("Output"));
            archive.Add(new XElement("FileSystem", "Yes"));
            archive.Add(new XElement("Microprogram"));
            archive.Add(new XElement("BootLoader"));
            archive.Add(new XElement("MpgmBootLoader"));
            archive.Add(new XElement("FPGA"));
            return archive;
        }

        private static XElement Microprogram()
        {
            var microprogram = new XElement("Microprogram");
            microprogram.Add(new XElement("Input"));
            microprogram.Add(new XElement("Output"));
            microprogram.Add(new XElement("ListFile"));
            microprogram.Add(new XElement("DebugInfo"));
            microprogram.Add(new XElement("AdditionalListFile"));
            microprogram.Add(new XElement("Version", "0.0.0.0"));
            return microprogram;
        }

        private static XElement Notes()
        {
            return new XElement("Notes", new XElement("Text"));
        }

        internal static XElement Generate(string name, string objectDir, string[] preprocessorDefinitions, string[] includeDirs)
        {
            var configuration = new XElement("Configuration");
            configuration.SetAttributeValue("Name", "Standard");
            configuration.Add(Debugger());
            configuration.Add(BootFiles());
            configuration.Add(CCompiler(objectDir, preprocessorDefinitions, includeDirs));
            configuration.Add(Linker(name));
            configuration.Add(Library());
            configuration.Add(Java());
            configuration.Add(Archive());
            configuration.Add(Microprogram());
            configuration.Add(Notes());
            return configuration;
        }
    }
}