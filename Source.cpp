using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Threading;

namespace LoadingStar {

    class Program {

        static void Main(string[] args) {

            IDictionary<string, string> argDict = new Dictionary<string, string>();

            for (int i = 0; i < args.Length; i += 2)
                argDict[args[i]] = args[i + 1];

            argDict.TryGetValue("--command", out string commandStr);

            bool waitingForCommand = !string.IsNullOrEmpty(commandStr);
            double milliseconds = 0;

            if (argDict.TryGetValue("-t", out string millisecondsStr))
                milliseconds = double.Parse(millisecondsStr);

            if (!string.IsNullOrEmpty(commandStr)) {

                ProcessStartInfo startInfo = new ProcessStartInfo {
                    Arguments = @"/C " + commandStr,
                    WindowStyle = ProcessWindowStyle.Hidden,
                    FileName = @"C:\Windows\System32\cmd.exe",
                    UseShellExecute = true,
                    WorkingDirectory = Directory.GetCurrentDirectory(),
                };

                new Thread(() => {

                    Process.Start(startInfo).WaitForExit();

                    waitingForCommand = false;

                }).Start();

            }

            DateTimeOffset startTime = DateTimeOffset.Now;
            string loadingChars = @"/-\|";
            int charIndex = 0;

            Console.CursorVisible = false;

            while (waitingForCommand || (DateTimeOffset.Now - startTime).TotalMilliseconds < milliseconds) {

                Console.Write(loadingChars[charIndex]);
                Console.CursorLeft -= 1;

                Thread.Sleep(200);

                charIndex = (charIndex + 1) % loadingChars.Length;

            }

            Console.Write(' ');
            Console.CursorLeft -= 1;
            Console.CursorVisible = true;

        }

    }
