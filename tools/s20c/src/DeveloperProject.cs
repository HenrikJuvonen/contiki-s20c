using System.IO;
using System.Linq;
using System.Xml.Linq;

namespace cc
{
    internal class DeveloperProject
    {
        private static XElement Profile(string name)
        {
            var profile = new XElement("Profile");
            profile.SetAttributeValue("Name", name);
            return profile;
        }

        private static XElement Configurations(string name, string objectDir, string[] preprocessorDefinitions, string[] includeDirs)
        {
            return new XElement("Configurations", Configuration.Generate(name, objectDir, preprocessorDefinitions, includeDirs));
        }

        private static XElement File(string path)
        {
            var file = new XElement("File");
            file.SetAttributeValue("Path", path);
            return file;
        }

        //private static string _root = (new FileInfo(System.Reflection.Assembly.GetEntryAssembly().Location)).Directory.Parent.Parent.FullName;

        private static XElement Folder(string name, string path, string[] files)
        {
            var groups = files.Select(n => n.Split(new[] { '/', '\\' }, 2)).Where(n => n.Length > 1).GroupBy(n => n[0]);
            var rootFiles = files.Select(n => n.Split(new[] { '/', '\\' }, 2)).Where(n => n.Length == 1).Select(n => n[0]);

            var f = new XElement("Folder");
            f.SetAttributeValue("Name", name);

            foreach (var group in groups)
            {
                var nm = group.First()[0];
                var fs = group.Select(n => n[1]).ToArray();
                f.Add(Folder(nm, Path.Combine(path, nm).Replace('/', '\\'), fs));
            }

            foreach (var file in rootFiles)
            {
                f.Add(File(Path.Combine(path, file).Replace('/', '\\')));
            }

            return f;
        }

        private static XElement Files(string root, string[] files)
        {
            var groups = files.Select(n => n.Split(new [] { '/', '\\' }, 2)).Where(n => n.Length > 1).GroupBy(n => n[0]);
            var rootFiles = files.Select(n => n.Split(new[] { '/', '\\' }, 2)).Where(n => n.Length == 1).Select(n => n[0]);

            var f = new XElement("Files");

            foreach (var group in groups)
            {
                var nm = group.First()[0];
                var fs = group.Select(n => n[1]).ToArray();
                f.Add(Folder(nm, Path.Combine(root, nm).Replace('/', '\\'), fs));
            }

            foreach (var file in rootFiles)
            {
                f.Add(File(Path.Combine(file).Replace('/', '\\')));
            }

            return f;
        }

        //private static XElement Files(string[] files)
        //{
        //    var f = new XElement("Files");

        //    foreach (var file in files)
        //    {
        //        f.Add(File(Path.Combine(file.Replace("/", "\\"))));
        //    }

        //    return f;
        //}

        private static XElement RamDiskFileSystem()
        {
            var ramDiskFileSystem = new XElement("RAMDiskFileSystem");
            //ramDiskFileSystem.SetAttributeValue("DynamicLink", "classes");
            return ramDiskFileSystem;
        }

        private static XElement ArchiveFileSystem()
        {
            return new XElement("ArchiveFileSystem");
        }

        private static XElement Microprogram()
        {
            var curTrigger = new XElement("CurTrigger");
            curTrigger.Add(new XElement("Name", "All"));
            curTrigger.Add(new XElement("Address", "0"));
            curTrigger.Add(new XElement("Dbus", "0"));
            curTrigger.Add(new XElement("YBus", "0"));
            curTrigger.Add(new XElement("AdrMask", "65535"));
            curTrigger.Add(new XElement("DBusMask", "255"));
            curTrigger.Add(new XElement("YBusMask", "255"));
            var microprogram = new XElement("Microprogram");
            microprogram.Add(curTrigger);
            return microprogram;
        }

        internal static XElement Generate(string name, string objectDir, string[] preprocessorDefinitions, string[] includeDirs, string root, string[] files)
        {
            var developerProject = new XElement("DeveloperProject");
            developerProject.SetAttributeValue("Version", "7.2.2");
            developerProject.SetAttributeValue("Name", name);
            developerProject.Add(Profile("IM3910-M210 v1.4.4"));
            developerProject.Add(Configurations(name, objectDir, preprocessorDefinitions, includeDirs));
            developerProject.Add(Files(root, files));
            //developerProject.Add(Files(files));
            developerProject.Add(RamDiskFileSystem());
            developerProject.Add(ArchiveFileSystem());
            developerProject.Add(Microprogram());
            return developerProject;
        }
    }
}