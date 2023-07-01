// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// Copyright (C) 2018 Henner Zeller <h.zeller@acm.org>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation version 2.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://gnu.org/licenses/gpl-2.0.txt>

#include "pixel-mapper.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include <map>

namespace rgb_matrix
{
    namespace
    {

        class RotatePixelMapper : public PixelMapper
        {
        public:
            RotatePixelMapper() : angle_(0 ) {}

            virtual const char *GetName() const { return "Rotate"; }

            virtual bool SetParameters( int chain, int parallel, const char *param)
            {
                if (param == NULL || strlen(param) == 0 )
                {
                    angle_ = 0;
                    return true;
                }
                char *errpos;
                const int angle = strtol(param, &errpos, 10 );
                if (*errpos != '\0')
                {
                    fprintf( stderr, "Invalid rotate parameter '%s'\n", param);
                    return false;
                }
                if (angle % 90 != 0 )
                {
                    fprintf( stderr, "Rotation needs to be multiple of 90 degrees\n");
                    return false;
                }
                angle_ = (angle + 360 ) % 360;
                return true;
            }

            virtual bool GetSizeMapping( int matrix_width, int matrix_height,
                                        int *visible_width, int *visible_height)
                const
            {
                if (angle_ % 180 == 0 )
                {
                    *visible_width = matrix_width;
                    *visible_height = matrix_height;
                }
                else
                {
                    *visible_width = matrix_height;
                    *visible_height = matrix_width;
                }
                return true;
            }

            virtual void MapVisibleToMatrix( int matrix_width, int matrix_height,
                                            int x, int y,
                                            int *matrix_x, int *matrix_y) const
            {
                switch (angle_)
                {
                case 0:
                    *matrix_x = x;
                    break;
                case 90:
                    *matrix_x = matrix_width - y - 1;
                    *matrix_y = x;
                    break;
                case 180:
                    *matrix_x = matrix_width - x - 1;
                    *matrix_y = matrix_height - y - 1;
                    break;
                case 270:
                    *matrix_x = y;
                    *matrix_y = matrix_height - x - 1;
                    break;
                }
            }

        private:
            int angle_;
        };

        class MirrorPixelMapper : public PixelMapper
        {
        public:
            MirrorPixelMapper() : horizontal_(true) {}

            virtual const char *GetName() const { return "Mirror"; }

            virtual bool SetParameters( int chain, int parallel, const char *param)
            {
                if (param == NULL || strlen(param) == 0 )
                {
                    horizontal_ = true;
                    return true;
                }
                if ( strlen(param) != 1)
                {
                    fprintf( stderr, "Mirror parameter should be a single "
                                    "character:'V' or 'H'\n");
                }
                switch (*param)
                {
                case 'V':
                case 'v':
                    horizontal_ = false;
                    break;
                case 'H':
                case 'h':
                    horizontal_ = true;
                    break;
                default:
                    fprintf( stderr, "Mirror parameter should be either 'V' or 'H'\n");
                    return false;
                }
                return true;
            }

            virtual bool GetSizeMapping( int matrix_width, int matrix_height,
                                        int *visible_width, int *visible_height)
                const
            {
                *visible_height = matrix_height;
                *visible_width = matrix_width;
                return true;
            }

            virtual void MapVisibleToMatrix( int matrix_width, int matrix_height,
                                            int x, int y,
                                            int *matrix_x, int *matrix_y) const
            {
                if (horizontal_)
                {
                    *matrix_x = matrix_width - 1 - x;
                }
                else
                {
                    *matrix_x = x;
                    *matrix_y = matrix_height - 1 - y;
                }
            }

        private:
            bool horizontal_;
    };

    /* //////////////////////////////////////// BEGIN OneSixtyFourMapper //////////////////////////////////////// */
    class OneSixtyFourMapper : public PixelMapper {
    public:
        OneSixtyFourMapper() : parallel_(1) { printf( "OneSixtyFourMapper constructed\n" ); }

        virtual const char *GetName() const { return "164-mapper"; }

        virtual bool SetParameters(int chain, int parallel, const char *param) {
            parallel_ = parallel;
            return true;
        }

        virtual bool GetSizeMapping(int matrix_width, int matrix_height,
                                    int *visible_width, int *visible_height) const {
            printf( "OneSixtyFourMapper::GetSizeMapping( %d, %d, %d, %d )\n", matrix_width, matrix_height, *visible_width, *visible_height );
            *visible_width = (matrix_width / 64) * 32;
            *visible_height = 2 * matrix_height;

            if (matrix_height % parallel_ != 0) {
                fprintf(stderr, "%s For parallel=%d we would expect the height=%d "
                        "to be divisible by %d ??\n", GetName(), parallel_, matrix_height, parallel_);
                return false;
            }
            return true;
        }

        virtual void MapVisibleToMatrix(int matrix_width, int matrix_height,
                                int x, int y, int *matrix_x, int *matrix_y) const {
            int panel_width = 64;
            int panel_height = 64;
            int panels_per_row = matrix_width / panel_width;

            int panel_index_x = x / panel_width;
            int panel_index_y = y / panel_height;

            int panel_x = panel_index_x * panel_width;
            int panel_y = panel_index_y * panel_height;

            *matrix_x = x % panel_width;
            *matrix_y = y % panel_height;

            if (*matrix_y >= 32) {
                *matrix_x += 32;
                *matrix_y -= 32;
            }

            *matrix_x += panel_index_x * panel_width;
            *matrix_y += panel_index_y * panel_height;

            // Ensure that the x-coordinate remains within the matrix_width
            if (*matrix_x >= matrix_width) {
                *matrix_x -= matrix_width;
            }
        }

    private:
        int parallel_;
    };



      
    //////////////////////////////////// END OneSixtyFourMapper ///////////////////////////////////////

                // int incoming_x = x;
                // int incoming_y = y;
                // Figure out what row and column panel this pixel is within.
                // int row = y / PANEL_HEIGHT; // _panel_height;
                // int col = x / PANEL_WIDTH;  // _panel_width;
                // int col = x / PANEL_WIDTH;
                // int panel_index = col * 2 + (y < PANEL_HEIGHT ? 0 : 1); // bot suggestion back in on tuesday after sleepy day.
                // int panel_index = ( COLS * row ) + col;
                // Panel panel = _panels[ panel_index ];  //_cols*row + col];
                // if ( x >= PANEL_WIDTH  ) { while ( x >= PANEL_WIDTH  ) { x -= PANEL_WIDTH;  }}
                // if ( y >= PANEL_HEIGHT ) { while ( y >= PANEL_HEIGHT ) { y -= PANEL_HEIGHT; }}
                // x_offset = (( CHAIN_LENGTH - 1 ) - panel.order ) * PANEL_WIDTH;  // this is the key line !!!  panel.order MATTERS !!!
                // 

                // rotations pasted below..
                // if ( panel.rotate == 180 ) {
                //     x = ( PANEL_WIDTH  - 1 ) - x;
                //     y = ( PANEL_HEIGHT - 1 ) - y;
                // }
/* //////////////////////////////////////// BEGIN TwoSixtyFourMapper //////////////////////////////////////// */
class TwoSixtyFourMapper : public PixelMapper {
            public:
            TwoSixtyFourMapper() : parallel_( 1 ) {
                //initialize the panels
                Panel firstPanel;
                firstPanel.name = "firstPanel";
                firstPanel.order = 2;
                firstPanel.rotate = 0;
                firstPanel.y_offset = 0;
                firstPanel.x_offset = 0;
                _panels[ 0 ] = firstPanel;

                Panel secondPanel;
                secondPanel.name = "secondPanel";
                secondPanel.order = 1;
                secondPanel.rotate = 0;
                _panels[ 1 ] = secondPanel;

                Panel thirdPanel;
                thirdPanel.name = "thirdPanel";
                thirdPanel.order = 0;
                thirdPanel.rotate = 0;
                _panels[ 2 ] = thirdPanel;

                Panel fourthPanel;
                fourthPanel.name = "fourthPanel";
                fourthPanel.order = 3;
                fourthPanel.rotate = 0;
                _panels[ 3 ] = fourthPanel;

                Panel fifthPanel;
                fifthPanel.name = "fifthPanel";
                fifthPanel.order = 4;
                fifthPanel.rotate = 0;
                _panels[ 4 ] = fifthPanel;

                Panel sixthPanel;
                sixthPanel.name = "sixthPanel";
                sixthPanel.order = 5;
                sixthPanel.rotate = 0;
                _panels[ 5 ] = sixthPanel;

                Panel seventhPanel;
                seventhPanel.name = "seventhPanel";
                seventhPanel.order = 6;
                seventhPanel.rotate = 0;
                _panels[ 6 ] = seventhPanel;

                Panel eighthPanel;
                eighthPanel.name = "eighthPanel";
                eighthPanel.order = 7;
                eighthPanel.rotate = 0;
                _panels[ 7 ] = eighthPanel; 
                printf( "264 mapper constructed. \n" ); }

            virtual const char *GetName() const { return "264-mapper"; }

            virtual bool SetParameters( int chain, int parallel, const char *param) { parallel_ = parallel; return true; } 
                                                        
            #define PANEL_WIDTH    32
            #define SLAB_HEIGHT    64
            #define MATRIX_WIDTH   128
            #define MATRIX_HEIGHT  32
            #define VISIBLE_HEIGHT 128
            #define VISIBLE_WIDTH  64
            #define CHAIN_LENGTH   4 // 8    // start mods for CHAIN_LENGTH...  started at 16 today
            #define ROWS           4
            #define COLS           2
            #define PANEL_PARALLEL 0
            #define PANEL_HEIGHT   32 // 64

            virtual bool GetSizeMapping(int matrix_width, int matrix_height,
                              int *visible_width, int *visible_height)
                const {
                *visible_width = matrix_width; 
                *visible_height = matrix_height;

                ///////////////////
                // below was altering the the visible variables!!
                //////////////////

                // *visible_width = (matrix_width / 64) * 32;   // Div at 32px boundary
                // *visible_height = 2 * matrix_height;
                // if (matrix_height % parallel_ != 0) {
                //     fprintf(stderr, "%s For parallel=%d we would expect the height=%d "
                //         "to be divisible by %d ??\n", GetName(), parallel_, matrix_height, parallel_ );
                //     return false; }

                // print visible width and height
                printf( "matrix width: %d  matrix height: %d \n", matrix_width, matrix_height );
                printf( "visible width: %d  visible height: %d \n", *visible_width, *visible_height );
                return true; }

            virtual void MapVisibleToMatrix(int matrix_width, int matrix_height,
                                            int x, int y, int *matrix_x, int *matrix_y) const {
                int incoming_x = x;
                int incoming_y = y;
                
                if ( incoming_y > 63 ) {
                    *matrix_x = incoming_x;
                    *matrix_y = incoming_y;
                } else {
                    *matrix_x = 64 - x - 1;
                    *matrix_y = 64 - y - 1;
                    // *matrix_x = incoming_x;
                    // *matrix_y = incoming_y;
                }

                // printf("Pixel: (%d, %d) Panel: (%d, %d)\n", x, y, *matrix_x, *matrix_y);
            }


            private:
            int parallel_;
            struct Panel {
                const char* name;
                int order    = 0;
                int rotate   = 0;
                int y_offset = 0;
                int x_offset = 0;
                // int parallel; hard code to 1
            };
            Panel _panels[ 16 ];
};        
//////////////////////////////////// END TwoSixtyFourMapper ///////////////////////////////////////



        // If we take a long chain of panels and arrange them in a U-shape, so
        // that after half the panels we bend around and continue below. This way
        // we have a panel that has double the height but only uses one chain.
        // A single chain display with four 32x32 panels can then be arranged in this
        // 64x64 display:
        //    [<][<][<][<] }- Raspbery Pi connector
        //
        // can be arranged in this U-shape
        //    [<][<] }----- Raspberry Pi connector
        //    [>][>]
        //
        // This works for more than one chain as well. Here an arrangement with
        // two chains with 8 panels each
        //   [<][<][<][<]  }-- Pi connector #1
        //   [>][>][>][>]
        //   [<][<][<][<]  }--- Pi connector #2
        //   [>][>][>][>]
        class UArrangementMapper : public PixelMapper {

            public:
            UArrangementMapper() : parallel_( 1 ) {

                //initialize the panels
                Panel firstPanel;
                firstPanel.name = "firstPanel";
                firstPanel.order = 5;
                firstPanel.rotate = 0;
                firstPanel.y_offset = 0;
                firstPanel.x_offset = 0;
                _panels[ 0 ] = firstPanel;

                Panel secondPanel;
                secondPanel.name = "secondPanel";
                secondPanel.order = 6;
                secondPanel.rotate = 0;
                _panels[ 1 ] = secondPanel;

                Panel thirdPanel;
                thirdPanel.name = "thirdPanel";
                thirdPanel.order = 7;
                thirdPanel.rotate = 0;
                _panels[ 2 ] = thirdPanel;

                Panel fourthPanel;
                fourthPanel.name = "fourthPanel";
                fourthPanel.order = 4;
                fourthPanel.rotate = 0;
                _panels[ 3 ] = fourthPanel;

                Panel fifthPanel;
                fifthPanel.name = "fifthPanel";
                fifthPanel.order = 1;
                fifthPanel.rotate = 0;
                _panels[ 4 ] = fifthPanel;

                Panel sixthPanel;
                sixthPanel.name = "sixthPanel";
                sixthPanel.order = 2;
                sixthPanel.rotate = 0;
                _panels[ 5 ] = sixthPanel;

                Panel seventhPanel;
                seventhPanel.name = "seventhPanel";
                seventhPanel.order = 3;
                seventhPanel.rotate = 0;
                _panels[ 6 ] = seventhPanel;

                Panel eighthPanel;
                eighthPanel.name = "eighthPanel";
                eighthPanel.order = 0;
                eighthPanel.rotate = 0;
                _panels[ 7 ] = eighthPanel;

                // Panel ninthPanel;
                // ninthPanel.name = "ninthPanel";
                // ninthPanel.order  = 7;
                // ninthPanel.rotate = 0;
                // _panels[ 8 ] = ninthPanel;

                // Panel tenthPanel;
                // tenthPanel.name = "tenthPanel";
                // tenthPanel.order  = 6;
                // tenthPanel.rotate = 0;
                // _panels[ 9 ] = tenthPanel;

                // Panel eleventhPanel;
                // eleventhPanel.name = "eleventhPanel";
                // eleventhPanel.order  = 5;
                // eleventhPanel.rotate = 0;
                // _panels[ 10 ] = eleventhPanel;

                // Panel twelfthPanel;
                // twelfthPanel.name = "twelfthPanel";
                // twelfthPanel.order  = 4;
                // twelfthPanel.rotate = 0;
                // _panels[ 11 ] = twelfthPanel;

                // Panel thirteenthPanel;
                // thirteenthPanel.name = "thirteenthPanel";
                // thirteenthPanel.order  = 3;
                // thirteenthPanel.rotate = 0;
                // _panels[ 12 ] = thirteenthPanel;

                // Panel fourteenthPanel;
                // fourteenthPanel.name = "fourteenthPanel";
                // fourteenthPanel.order  = 2;
                // fourteenthPanel.rotate = 0;
                // _panels[ 13 ] = fourteenthPanel;

                // Panel fifteenthPanel;
                // fifteenthPanel.name = "fifteenthPanel";
                // fifteenthPanel.order  = 1;
                // fifteenthPanel.rotate = 0;
                // _panels[ 14 ] = fifteenthPanel;

                // Panel sixteenthPanel;
                // sixteenthPanel.name = "sixteenthPanel";
                // sixteenthPanel.order  = 0;
                // sixteenthPanel.rotate = 0; // start mods...
                // _panels[ 15 ] = sixteenthPanel;
            }

            virtual const char *GetName() const { return "U-mapper"; }

            virtual bool SetParameters( int chain, int parallel, const char *param) {
                // if (chain < 2) {  // technically, a chain of 2 would work, but somewhat pointless
                //     fprintf( stderr, "U-mapper: need at least --led-chain=4 for useful folding\n");
                //     return false;
                // }
                // if (chain % 2 != 0 ) {
                // fprintf( stderr, "U-mapper: Chain (--led-chain) needs to be divisible by two\n");
                // return false;
                // }
                parallel_ = parallel;
                return true;
            } 
                                                        
            // #define PANEL_HEIGHT   32
            #define PANEL_WIDTH    32
            #define SLAB_HEIGHT    64
            #define MATRIX_WIDTH   128
            #define MATRIX_HEIGHT  32
            #define VISIBLE_HEIGHT 128
            #define VISIBLE_WIDTH  64
            // #define CHAIN_LENGTH   8    // start mods for CHAIN_LENGTH...  started at 16 today
            #define ROWS           4
            #define COLS           2
            #define PANEL_PARALLEL 0

            virtual bool GetSizeMapping( int matrix_width, int matrix_height, int *visible_width, int *visible_height )
                const {
                printf( "inside virtual bool GetSizeMapping: matrix_width=%d, matrix_height=%d, *visible_width=%d, *visible_height=%d\n", matrix_width, matrix_height, *visible_width, *visible_height );
                *visible_width = ( matrix_width / MATRIX_WIDTH /* before doubling chain, 64 */ ) * 32;   // Div at 32px boundary
                *visible_height = ROWS /* before doubling chain, 2 */ * matrix_height;
                if ( matrix_height % parallel_ != 0 ) { fprintf( stderr, "%s For parallel=%d we would expect the height=%d to be divisible by %d ??\n", GetName(), parallel_, matrix_height, parallel_ );
                    return false; }

                printf( "After altering: visible_width: %d,  visible_height: %d \n", *visible_width, *visible_height );
                printf( "returning true from virtual bool GetSizeMapping in pixel-mapper.cc ... \n\n"             );
                return true; }

            virtual void MapVisibleToMatrix( int mw, int mH, int x, int y, int *matrix_x, int *matrix_y ) const {

                // Figure out what row and column panel this pixel is within.
                int row = y / PANEL_HEIGHT; // _panel_height;
                int col = x / PANEL_WIDTH;  // _panel_width;
                
                // Compute the index of the panel in the panel list.
                int panel_index = ( COLS * row ) + col;
                // printf( "panel_index: %d  ", panel_index );

                //Get the panel information for this pixel.
                Panel panel = _panels[ panel_index ];  //_cols*row + col];
                // printf( "input coordinates( %3d, %3d ) row: %d  col: %d  panel.order: %2d, panel.rotate: %3d panel.name: %12s ", x, y, row, col, panel.order, panel.rotate, panel.name );
                // printf( "x: %2d  y: %2d ", x, y );

                // Compute location of the pixel within the panel.
                // x = x % PANEL_WIDTH;  // _panel_width;
                // y = y % PANEL_HEIGHT; // _panel_height;

                if ( x >= PANEL_WIDTH  ) { while ( x >= PANEL_WIDTH  ) { x -= PANEL_WIDTH;  }}
                if ( y >= PANEL_HEIGHT ) { while ( y >= PANEL_HEIGHT ) { y -= PANEL_HEIGHT; }}


                //////////////////////////////map single panel to matrix ///////////////////////////////////////////////////
                // int panel_rows_ = PANEL_HEIGHT;
                // int panel_cols_ = PANEL_WIDTH;
                // int single_panel_matrix_x = 0;
                // int single_panel_matrix_y = 0;

                // const bool is_top_check = (y % (panel_rows_/2)) < panel_rows_/4;
                // const bool is_left_check = (x < panel_cols_/2);
                // if (is_top_check) {
                //     single_panel_matrix_x = is_left_check ? x+panel_cols_/2 : x+panel_cols_;
                // } else {
                //     single_panel_matrix_x = is_left_check ? x : x + panel_cols_/2;
                // }
                // single_panel_matrix_y = ((y / (panel_rows_/2)) * (panel_rows_/4) + y % (panel_rows_/4));

                // x = single_panel_matrix_x;
                // y = single_panel_matrix_y;
                /////////////////////////////////////////////////////////////////////////////////////////////////////////////


                // Perform any panel rotation to the pixel.
                // NOTE: 90 and 270 degree rotation only possible on 32 row ( square) panels.
                if ( panel.rotate == 90 ) {
                    // assert(_panel_height == _panel_width); // asserted.  PANEL_HEIGHT == PANEL_WIDTH
                    int old_x = x;
                    x = ( PANEL_HEIGHT - 1 ) - y;
                    y = old_x;
                }
                else if ( panel.rotate == 180 ) {
                    x = ( PANEL_WIDTH  - 1 ) - x;
                    y = ( PANEL_HEIGHT - 1 ) - y;
                }
                else if ( panel.rotate == 270 ) {
                    // assert( PANEL_HEIGHT == PANEL_WIDTH ); // asserted.  PANEL_HEIGHT == PANEL_WIDTH
                    int old_y = y;
                    y = ( PANEL_WIDTH - 1 ) - x;
                    x = old_y;
                }

                // Determine x offset into the source panel based on its order along the chain.
                // The order needs to be inverted because the matrix library starts with the
                // origin of an image at the end of the chain and not at the start (where
                // ordering begins for this transformer).
                int x_offset, y_offset = 0;
                
                x_offset = (( CHAIN_LENGTH - 1 ) - panel.order ) * PANEL_WIDTH;  // this is the key line !!!  panel.order MATTERS !!!
                y_offset = panel.y_offset;
               
                
                // Determine y offset into the source panel based on its parrallel chain value.

                // _source->SetPixel(x_offset + x,
                //                     y_offset + y,
                //                     red, green, blue);

                // if ( x < 63 && y >  ) {
                //     // x_offset = x + panel.x_offset;
                //     // y_offset = y + panel.y_offset;
                //     y_offset = -32;
                // } ele

                *matrix_x = x + x_offset;
                *matrix_y = y + y_offset;
                // printf( " ( %2d, %2d ) x_offset: %3d  y_offset: %3d  matrix_x: %3d  matrix_y%3d \n", x, y, x_offset, y_offset, *matrix_x, *matrix_y );

                // *matrix_x = x;
                // *matrix_y = /*base_y +*/ y;
                    
                // printf( "pnl_hght: %d  vsbl_wdth: %d  slb_hght: %d bs_y: %d mtrx_wdth: %d mtrx_hght: %d, mtrx_x: %d,   mtrx_y %d \n", 
                //          MATRIX_HEIGHT, VISIBLE_WIDTH, SLAB_HEIGHT, base_y,  matrix_width, matrix_height, *matrix_x,    *matrix_y );                
            }

            private:
            int parallel_;
            struct Panel {
                const char* name;
                int order    = 0;
                int rotate   = 0;
                int y_offset = 0;
                int x_offset = 0;
                // int parallel; hard code to 1
            };
            Panel _panels[ 16 ];
        };        
       
/////////////////////////////////////////// End of U-mapper ///////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////
        

        class VerticalMapper : public PixelMapper
        {
        public:
            VerticalMapper() {}

            virtual const char *GetName() const { return "V-mapper"; }

            virtual bool SetParameters( int chain, int parallel, const char *param)
            {
                chain_ = chain;
                parallel_ = parallel;
                // optional argument :Z allow for every other panel to be flipped
                // upside down so that cabling can be shorter:
                // [ O < I ]   without Z       [ O < I  ]
                //   ,---^      <----                ^
                // [ O < I ]                   [ I > O  ]
                //   ,---^            with Z     ^
                // [ O < I ]            --->   [ O < I  ]
                z_ = (param && strcasecmp(param, "Z") == 0 );
                return true;
            }

            virtual bool GetSizeMapping( int matrix_width, int matrix_height,
                                        int *visible_width, int *visible_height)
                const
            {
                *visible_width = matrix_width * parallel_ / chain_;
                *visible_height = matrix_height * chain_ / parallel_;
#if 0
     fprintf( stderr, "%s: C:%d P:%d. Turning W:%d H:%d Physical "
	     "into W:%d H:%d Virtual\n",
             GetName(), chain_, parallel_,
	     *visible_width, *visible_height, matrix_width, matrix_height);
#endif
                return true;
            }

            virtual void MapVisibleToMatrix( int matrix_width, int matrix_height,
                                            int x, int y,
                                            int *matrix_x, int *matrix_y) const
            {
                const int panel_width = matrix_width / chain_;
                const int panel_height = matrix_height / parallel_;
                const int x_panel_start = y / panel_height * panel_width;
                const int y_panel_start = x / panel_width * panel_height;
                const int x_within_panel = x % panel_width;
                const int y_within_panel = y % panel_height;
                const bool needs_flipping = z_ && (y / panel_height) % 2 == 1;
                *matrix_x = x_panel_start + (needs_flipping
                                                 ? panel_width - 1 - x_within_panel
                                                 : x_within_panel);
                *matrix_y = y_panel_start + (needs_flipping
                                                 ? panel_height - 1 - y_within_panel
                                                 : y_within_panel);
            }

        private:
            bool z_;
            int chain_;
            int parallel_;
        };

        typedef std::map<std::string, PixelMapper *> MapperByName;
        static void RegisterPixelMapperInternal(MapperByName *registry,
                                                PixelMapper *mapper)
        {
            assert(mapper != NULL);
            std::string lower_name;
            for (const char *n = mapper->GetName(); *n; n++)
                lower_name.append(1, tolower(*n));
            (*registry)[lower_name] = mapper;
        }

        static MapperByName *CreateMapperMap() {
            MapperByName *result = new MapperByName();

            // Register all the default PixelMappers here.
            RegisterPixelMapperInternal(result, new RotatePixelMapper());
            RegisterPixelMapperInternal(result, new UArrangementMapper());
            RegisterPixelMapperInternal(result, new VerticalMapper());
            RegisterPixelMapperInternal(result, new MirrorPixelMapper());
            RegisterPixelMapperInternal(result, new TwoSixtyFourMapper());
            RegisterPixelMapperInternal(result, new OneSixtyFourMapper());
            return result;
        }

        static MapperByName *GetMapperMap()
        {
            static MapperByName *singleton_instance = CreateMapperMap();
            return singleton_instance;
        }
    } // anonymous namespace

    // Public API.
    void RegisterPixelMapper(PixelMapper *mapper)
    {
        RegisterPixelMapperInternal(GetMapperMap(), mapper);
    }

    std::vector<std::string> GetAvailablePixelMappers()
    {
        std::vector<std::string> result;
        MapperByName *m = GetMapperMap();
        for (MapperByName::const_iterator it = m->begin(); it != m->end(); ++it)
        {
            result.push_back(it->second->GetName());
        }
        return result;
    }

    const PixelMapper *FindPixelMapper(const char *name,
                                       int chain, int parallel,
                                       const char *parameter)
    {
        std::string lower_name;
        for (const char *n = name; *n; n++)
            lower_name.append(1, tolower(*n));
        MapperByName::const_iterator found = GetMapperMap()->find(lower_name);
        if (found == GetMapperMap()->end())
        {
            fprintf( stderr, "%s: no such mapper\n", name);
            return NULL;
        }
        PixelMapper *mapper = found->second;
        if (mapper == NULL)
            return NULL; // should not happen.
        if (!mapper->SetParameters(chain, parallel, parameter))
            return NULL; // Got parameter, but couldn't deal with it.
        return mapper;
    }
} // namespace rgb_matrix
