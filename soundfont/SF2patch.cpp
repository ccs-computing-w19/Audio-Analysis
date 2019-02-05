#include <stdio.h>
void SF2patch(unsigned char overwrite)
{
    FILE *fp;
    fp = fopen("GMGSx_nnn.sf2", "r+b"); // open file
    unsigned short int offsets[110]={630, 614, 610, 610, 610, 610, 618, 258, 246, 254, 194, 238, 382, 382, 182, 446, 282, 298, 286, 298, 242, 138, 194, 194, 290, 290, 130, 130, 126, 130, 182, 206, 126, 134, 122, 122, 134, 158, 258, 130, 190, 194, 274, 274, 366, 178, 242, 418, 318, 362, 374, 370, 366, 382, 242, 362, 406, 378, 234, 266, 274, 274, 234, 474, 422, 422, 62, 182, 302, 246, 362, 390, 106, 182, 190, 134, 194, 286, 246, 282, 574, 562, 658, 494, 414, 214, 342, 558, 202, 190, 202, 178, 318, 218, 198, 198, 206, 254, 550, 546, 514, 458, 382, 302, 278, 182, 174, 286, 346, 286}; // offsets
    fseek(fp, 0x3DD898, SEEK_SET); // Move pointer to first offset
    fwrite(&overwrite, sizeof(overwrite), 1, fp); // Overwriting bytes
    fwrite(&overwrite, sizeof(overwrite), 1, fp);
    for (int i = 0;i<110;i++) {
      // looping through offsets and overwriting bytes
      fseek(fp, offsets[i], SEEK_CUR); // moving pointer to next offset
      fwrite(&overwrite, sizeof(overwrite), 1, fp);  // Overwriting bytes
      fwrite(&overwrite, sizeof(overwrite), 1, fp);
    }
    fclose(fp); // close file
    return;
}
