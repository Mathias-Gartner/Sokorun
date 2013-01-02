Dim gitPath
gitPath = """D:\Program Files (x86)\Git\bin\"

dim scriptShell
Set scriptShell = CreateObject("WScript.Shell")

Dim fileSystemObject
Set fileSystemObject = CreateObject("Scripting.FileSystemObject")

Dim mainFolder
Set mainFolder = fileSystemObject.GetFolder("level")

Dim subFolders
Set subFolders = mainFolder.SubFolders

For Each folder in subFolders

  Dim files
  Set files = folder.Files

  For Each file in files

    If ( Mid(file, Len(file)-3, 4) = ".txt" ) Then

      scriptShell.Run gitPath + "git.exe""" + " mv """ + file + """ """ + Left(file, Len(file)-4) + ".lvl"""
    
    End If
    
  Next

Next

