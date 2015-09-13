using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Threading;

namespace cc
{
    internal class Program
    {
        private static void Main(string[] args)
        {
            var name = "";
            var includeDirs = new List<string>();
            var files = new List<string>();
            var objectDir = "";
            var rootDir = "";
            var definitions = new List<string>();
            var uccPath = @"C:\Program Files (x86)\Cimsys\Cimsys Developer\bin\ucc";
            var profilePath = @"C:\Program Files (x86)\Imsys Technologies\Profiles\IM3910-M210 v1.4.4";

            if (args.Length == 0)
            {
                Logger.WriteLine("Usage:");
                Logger.WriteLine("  -n <name>");
                Logger.WriteLine("  -i <include-dir> <include-dir> ...");
                Logger.WriteLine("  -f <source-file> <source-file> ...");
                Logger.WriteLine("  -o <object-dir>");
                Logger.WriteLine("  -r <root-dir>");
                Logger.WriteLine("  -d <preprocessor-definition> <preprocessor-definition> ... ");
                Logger.WriteLine("  -u <ucc-path> (default: " + uccPath + ")");
                Logger.WriteLine("  -p <profile-path> (default: " + profilePath + ")");
                return;
            }

            var c = 0;
            foreach (var arg in args)
            {
                switch (arg)
                {
                    case "-n":
                        c = 1;
                        name = "";
                        break;
                    case "-i":
                        c = 2;
                        break;
                    case "-f":
                        c = 3;
                        break;
                    case "-o":
                        c = 4;
                        objectDir = "";
                        break;
                    case "-r":
                        c = 5;
                        rootDir = "";
                        break;
                    case "-d":
                        c = 6;
                        break;
                    case "-u":
                        c = 7;
                        uccPath = "";
                        break;
                    case "-p":
                        c = 8;
                        profilePath = "";
                        break;
                    default:
                        switch (c)
                        {
                            case 1:
                                name += arg;
                                break;
                            case 2:
                                includeDirs.Add(arg);
                                break;
                            case 3:
                                files.Add(arg);
                                break;
                            case 4:
                                objectDir += arg;
                                break;
                            case 5:
                                rootDir += arg;
                                break;
                            case 6:
                                definitions.Add(arg);
                                break;
                            case 7:
                                uccPath += arg;
                                break;
                            case 8:
                                profilePath += arg;
                                break;
                        }
                        break;
                }
            }

            name = name.Trim('"', '\'');
            objectDir = objectDir.Trim('"', '\'');
            rootDir = rootDir.Trim('"', '\'');
            uccPath = uccPath.Trim('"', '\'');
            profilePath = profilePath.Trim('"', '\'');

            rootDir = rootDir.Replace('/', '\\');

            var sw = Stopwatch.StartNew();
            Logger.WriteLine("Build started: {0}", name);
            
            var dir = Directory.GetDirectories(rootDir, "*", SearchOption.AllDirectories);

            files = files.Select(n =>
            {
                var arr = dir.Select(d => Path.Combine(d, n)).Where(File.Exists).ToArray();
                if (!arr.Any())
                {
                    Logger.WriteLine(ConsoleColor.Red, "File not found: " + n);
                    Environment.Exit(1);
                }
                return arr.First();
            }).Select(n => n.Replace('\\', '/')).Select(n => n.Remove(0, rootDir.Length + 1)).ToList();

            includeDirs = includeDirs.Select(n => n.Replace('\\', '/')).ToList();

            var buildSystem = new BuildSystem(name, includeDirs, files, objectDir, rootDir, definitions, uccPath, profilePath);
            buildSystem.GenerateProjectFile();
            buildSystem.CompileAndAssemble();
            buildSystem.Link();
            buildSystem.Debug();

            sw.Stop();
            Logger.WriteLine("Build finished, duration: {0} ms", sw.ElapsedMilliseconds);
            Thread.Sleep(200);
        }
    }
}