#include "hulib.h"
#include "codec.h"
#include "HumapXML.h"
#include <sys/types.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

HUMap res_index;
FILE * fp = NULL;


void huErrExit(char const* str)
{
	fprintf(stderr,"%s",str);
	exit(-1);
}
void Dir(hustr dir, int l)
{
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	printf("open %s\r\n", dir.c_str());
	if ((dp = opendir(dir)) == NULL)
	{
		fprintf(stderr, "cannot open directory: %s\n", dir.c_str());
		return;
	}
	while ((entry = readdir(dp)) != NULL)
	{

		if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
			continue;

		//printf("%*s%s/\n", depth, "", entry->d_name);

		hustr subfile = dir;
		subfile += "/";
		subfile += entry->d_name;
		lstat(subfile, &statbuf);
		if (S_ISDIR(statbuf.st_mode) && l > 0)
		{
			Dir(subfile, l - 1);
		}
		else
		{
			char * p = strrchr(entry->d_name, '.');
			if (p != NULL)
			{
				char * type = p+1;
				if (strcasecmp(type,"png")==0)
				{
					image tmp;
					hustr file("%s/%s", dir.nstr(), entry->d_name);
					tmp.Load(file);
					const char * name = file.nstr();
					if(name[0]=='.'&&name[1]=='/')
						name += 2;
					HUMap & index = res_index.CreateMultiLast("png");
					//HUMap & index = res_index[hustr("\"%s\"",name)];
					index["name"]->format("%s",name);
					index["width"]->format("%d",tmp.GetImageWidth());
					index["height"]->format("%d",tmp.GetImageHeight());
					index["stride"]->format("%d",tmp.GetImageStride());
					index["offset"]->format("%d",ftell(fp));

					fwrite(tmp.GetAddr(), tmp.getSize(), 1,fp);
				}
			}

		}

		//printf("%*s%s\n", depth, "", entry->d_name);

	}
	closedir(dp);
}

int main(int argc,char * argv[])
{
	res_index = "res_index";
	fp = fopen("./hu.res","w");
	fseek(fp, 0, SEEK_SET);
	Dir(".",999);
	res_index.display();
	HumapToXmlFile("hu.res.xml",res_index);
	fclose(fp);
}
