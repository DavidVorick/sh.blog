#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

int main() {
	FILE* index_hints = fopen("htdocs/guide_index.txt", "r"); //tells the c program how to write the index page
	FILE* categories = fopen("htdocs/guide_categories.txt", "r"); //tells the c program which category index means what
	FILE* index = fopen("htdocs/0_0.sbf", "w"); //this is the index file for the website
	FILE* all = fopen("htdocs/0_1.sbf", "w"); //this page displays the full list of blog entries on the site
	FILE* images = fopen("htdocs/0_2.sbf", "w"); //this page displays every image that is used in any blog post`
	
	//I use lighttpd, which puts public http files in the 'htdocs' folder
	//I keep the c file in the level above, so that only people with shell access can run the file
	DIR* htdocs = opendir("htdocs");
	struct dirent *ent;
	
	//sizes are arbitrary, little rationale behind them
	char filenames[250][250];
	char last_modified[5000];
	char linebuffer[5000];
	char buffer2[5000];
	
	//read the first 3 lines of the index_hints file and write them to the index page
	  //line 1 contains the page title
	  //line 2 contains the date header
	  //line 3 contains the date footer (usually empty)
	int i;
	for(i=0; i<3; i++) {
		fgets(linebuffer, sizeof(linebuffer), index_hints);
		fputs(linebuffer, index);

		if(i == 1) //the date header
			snprintf(last_modified, sizeof(last_modified), "%s", linebuffer);
	}
	
	//the index_hints file lists multiple other sbf files, the contents of each will be added to the index page
	//each iteration of this loop adds another file's contents to the index page
	while(fgets(linebuffer, sizeof(linebuffer), index_hints) != NULL) {
		//remove the trailing newline character
		i = strcspn(linebuffer,"\n");
		linebuffer[i] = '\0';
		
		//open the file listed in index_hints
		snprintf(buffer2, sizeof(buffer2), "htdocs/%s", linebuffer);
		FILE* file_preview = fopen(buffer2, "r");
		
		//skip the first 3 lines (header information)
		for(i=0; i<3; i++)
			fgets(linebuffer, sizeof(linebuffer), file_preview);
		
		//write all of the remaining lines
		while(fgets(linebuffer, sizeof(linebuffer), file_preview) != NULL)
			fputs(linebuffer, index);
		
		fclose(file_preview);
	}
	fclose(index);

	//write the header information for the meta-pages
	fputs("All Pages\n", all);
	fputs("All Images\n", images);
	fputs(last_modified, all);
	fputs(last_modified, images);
	fputs("\n", images);
	
	//read through the htdocs directory and store all the filenames in an array
	i = 0;
	while((ent = readdir(htdocs)) != NULL) {
		strcpy(filenames[i], ent->d_name);
		i++;
	}
	
	//The outermost while loop reads through the categories file, to divide the 'all' page by category
	//I am unhappy with this entire loop body and intend to refactor it at some point. Maybe you can help?
	int j = 1;
	while(fgets(linebuffer, sizeof(linebuffer), categories) != NULL) {
		snprintf(buffer2, sizeof(buffer2), "\nh: %sp: ", linebuffer);
		fputs(buffer2, all);

		//this for loop goes through all files each time per category
		//it's necessary because the filenames are not sorted
		int k;
		for(k=0; k<i; k++) {
			//check that the file starts with a character indicating the correct category
			if((filenames[k][0] - '0') == j) {
				//open the file and read the first line
				snprintf(linebuffer, sizeof(linebuffer), "htdocs/%s", filenames[k]);
				FILE* title_grabber = fopen(linebuffer, "r");
				fgets(buffer2, sizeof(buffer2), title_grabber);
				
				//parse the filename and retrieve the post-number
				int l = 2;
				int category_value = 0;
				while(isdigit(filenames[k][l])) {
					category_value *= 10;
					category_value += (filenames[k][l] - '0');
					l++;
				}
				
				//strip the newline
				l = strcspn(buffer2, "\n");
				buffer2[l] = '\0';
				
				//write the link. This produces unreadable html, but adding a newline to 
					//fix the html trips up the snprintf function.
				snprintf(linebuffer, sizeof(linebuffer), "<a href=\"index.php?category=%u&index=%u\">%s</a><br>", j, category_value, buffer2);
				fputs(linebuffer, all);
				fclose(title_grabber);

				//the else statement checks if the file is an image file
				//j == 0 insures that each image is only printed once
				//the prefix dvp is meant to be specific to me (david vorick something, I forget)
			} else if(filenames[k][0]  == 'd' && j == 0 && filenames[k][7] == '.') {
				snprintf(linebuffer, sizeof(linebuffer), "i: %c%c%c\n", filenames[k][4], filenames[k][5], filenames[k][6]);
				fputs(linebuffer, images);
			}
		}
		j++;
	}

	fclose(categories);
	fclose(all);
	fclose(images);
	fclose(index_hints);
	return 0;
}
