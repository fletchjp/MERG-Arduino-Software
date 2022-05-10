# GitHub Reflection

This is a page for reflection on my learning experience using this system.

I have been learning more about how to use GitHub. In particular one of the repositories I have forked had been updated. I have been learning how to keep my fork up to date. My source for this has been the video of Sven Rosvall's talk.

I think it is worth recording where I started at the beginning of this work. I had used git before some years ago and had not used GitHub then. I met GitHub when I started to look into using MERG-DEV which was already the site for MERG CBUS PIC codes and some other things.

I set up this site in August 2020 and did little with it. I did some more in late November 2020. I really only got going seriously in February 2021 with the first of Sven's talks and really have collaboration running only since his second (third) talk.

## Experience

I have been setting up branches to sort out CANCMDDC2. I set up some branches and decided to copy one into main using a pull request. I attempted to remove conflict by renaming BRANCHES.md in main and deleting the file. I could not merge that and needed to create a new file with the same name to complete the merge.

gitg shows branches usefully. It needs to be restarted to show all the branch tags properly.

I have now become familiar with updating a repository I have forked by updating the branch in GitHub desktop.

I have on one occasion cherrypicked between branches.

I have also learned how to delete a repository, one which I had forked and no longer needed.

I had a strange experience today. I was working in the cbus_codes branch of MERG-Arduino-Software where I had two codes open to change things. I wanted to compare them with the code for CANCMDDC which is in the main branch of CANCMDDC2. So I changed directory in the open Explorer page and found it and opened it. When I came to save it I was back in the folder for MERG-Arduino-Software it was not there. Panic. Where is it? I saved a backup somewhere outside the GitHub structure. Then I realised it came from a different repository. The files were still there from last time I had used that repository. End of panic.

## Changing branches around

I have done some interesting things with the CANCMDDC2 repository. I had been working in a branch called develop and wanted to make that the main branch. I started by renaming the existing main to old-main and then renaming develop to main. I also needed to change the default branch to the new main.

In the course of doing this and getting it sorted out in the GitHub desktop I have ended up with what I wanted. I now have a main which inherits from both old-main and develop and I also still have a develop I can start to use for a new version.

## VS Code configuration

I have had a lot of problems getting VS Code configured and at times I despaired of making sense of it. I have stuck with it and have learned quite a lot about how it works. There is still more to go.

In the end I have abandoned using VS Code, mainly because it took up too much memory on the main computer, where I am short of space on the C: drive.

There are configuration files lying around.

I am using the Arduino IDE directly.

Back to [README](README.md).

