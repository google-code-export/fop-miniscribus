
#include "lcms.h"

//
//      This snippet shows how to deal with colorimetric chunks in PNG files;
//      It is intended only as an example. The returned profile can be used
//      as input profile for color match transforms.
//

cmsHPROFILE PNGGetColorProfile(void)
{

       // First try to see if iCCP chunk is present

       if (png_get_valid(png_ptr, info_ptr, PNG_INFO_iCCP))
       {

              DWORD ProfileLen;
              char *ProfileData;
              int  Compression;
              char *ProfileName;

              png_get_iCCP(png_ptr, info_ptr, &ProfileName,
                                              &Compression,
                                              &ProfileData,
                                              &ProfileLen);

              return cmsOpenProfileFromMem(ProfileData,
                                           ProfileLen);
       }


       // Try to see if sRGB is specified, then use it

       if (png_get_valid(png_ptr, info_ptr, PNG_INFO_sRGB))
       {
              char sRGBFile[MAX_PATH], SysDir[MAX_PATH];

              GetSystemDirectory(SysDir, MAX_PATH-1);
              sprintf(sRGBFile, "%s\\COLOR\\sRGB Color Space Profile.ICM",
                                                                      SysDir);

              if (access(sRGBFile, 0) == 0)
                     return cmsOpenProfileFromFile(sRGBFile, "r");
       }


       // Then, try to see if chromacities are defined.

       if (png_get_valid(png_ptr, info_ptr, PNG_INFO_cHRM) &&
           png_get_valid(png_ptr, info_ptr, PNG_INFO_gAMA))
           {
           cmsCIExyY WhitePoint;
           cmsCIExyYTRIPLE Primaries;
           cmsHPROFILE hReturn;

           png_get_cHRM(png_ptr, info_ptr,
                            &WhitePoint.x, &WhitePoint.y,
                            &Primaries.Red.x,   &Primaries.Red.y,
                            &Primaries.Green.x, &Primaries.Green.y,
                            &Primaries.Blue.x,  &Primaries.Blue.y);

           WhitePoint.Y =
           Primaries.Red.Y =
           Primaries.Green.Y =
           Primaries.Blue.Y  = 1.0;

           double GammaOfFile;
           LPGAMMATABLE GammaTable[3];

           png_get_gAMA(png_ptr, info_ptr, &GammaOfFile);

           GammaTable[0] = GammaTable[1] =
           GammaTable[2] = cmsBuildGamma(256, 1/GammaOfFile);

           if (!GammaTable[0]])
              SignalError(-1, "Create gammatable: cannot allocate");

           hReturn = cmsCreateRGBProfile(&WhitePoint, &Primaries, GammaTable);

           cmsFreeGamma(GammaTable[0]);

           return hReturn;
           }

           // Only gamma is given, assume sRGB primaries??

           if (png_get_valid(png_ptr, info_ptr, PNG_INFO_gAMA))
           {
           double GammaOfFile;
           LPGAMMATABLE GammaTable[3];
           cmsCIExyY WhitePoint =  {0.3127, 0.3290, 1.0};         // D65
           cmsHPROFILE hReturn;
           cmsCIExyYTRIPLE Primaries =
                                   {
                                   {0.6400, 0.3300, 1.0},
                                   {0.3000, 0.6000, 1.0},
                                   {0.1500, 0.0600, 1.0}
                                   };

           png_get_gAMA(png_ptr, info_ptr, &GammaOfFile);

           GammaTable[0] = GammaTable[1] =
           GammaTable[2] = cmsBuildGamma(256, 1/GammaOfFile);

           if (!GammaTable[0])
              SignalError(-1, "Create gammatable: cannot allocate");

           hReturn = cmsCreateRGBProfile(&WhitePoint, &Primaries, GammaTable);

           cmsFreeGamma(GammaTable[0]);
           return hReturn;
           }


       // The image cannot be accurately described

       return NULL;

}

