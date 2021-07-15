#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <time.h>

#include "colour.h"
#include "config.h"
struct dist {
	char *col1, *col2, *col3, *col4, *col5, *col6, *col7, *col8;
	char *getPkgCount;
};
struct dist info = {
	.col1 = BMAGENTA "     _   \n   ",
	.col2 = BCYAN " _/   \\___   ",
	.col3 = BYELLOW "/   '---'   \\ ",
	.col4 = BGREEN "'--___--'         ",
	.col5 = BCYAN "     / \\         ",
	.col6 = BMAGENTA "    /   \\       ",
	.col7 = BGREEN "   /     \\        ",
	.col8 = BYELLOW "",
	.getPkgCount = "echo unsupported",
};
char *username="\t: Basit/Eqan";
char *osname="\t: Xaxis OS";
char *krnlver="\t: Operating Systems Final";
long int uptimeH=675;
char *shellname=": C/C++, BASH, GTK";
char *pkgCount="\t: 74";
int main(){
    printf("%s", info.col1);
	printf("%s    %s%s%s\n", info.col2, UserText, TextColour, username);
    printf("%s    %s%s%s\n", info.col3, OsText, TextColour, osname);
    printf("%s    %s%s%s\n", info.col4, KernelText, TextColour, krnlver);
    printf("%s    %s%s%s\n", info.col5, ShellText, TextColour, shellname);
	printf("%s    %s%s%s\n", info.col6, PackageText, TextColour, pkgCount);
    printf("%s    %s%s%ldh\n", info.col7, UptimeText, TextColour, uptimeH);
	printf("%s\n", info.col8);
    printf("%s", RESET);
}