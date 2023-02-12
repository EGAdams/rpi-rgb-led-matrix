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
        class UArrangementMapper : public PixelMapper  {
        public:
            UArrangementMapper() : parallel_(1) {}

            virtual const char *GetName() const { return "U-mapper"; }

            virtual bool SetParameters(int chain, int parallel, const char *param)
            {
                if (chain < 2)
                { // technically, a chain of 2 would work, but somewhat pointless
                    fprintf(stderr, "U-mapper: need at least --led-chain=4 for useful folding\n");
                    return false;
                }
                if (chain % 2 != 0)
                {
                    fprintf(stderr, "U-mapper: Chain (--led-chain) needs to be divisible by two\n");
                    return false;
                }
                parallel_ = parallel;
                return true;
            }

            virtual bool GetSizeMapping(int matrix_width, int matrix_height,
                                        int *visible_width, int *visible_height)
                const
            {
                *visible_width = (matrix_width / 64) * 32; // Div at 32px boundary
                *visible_height = 2 * matrix_height;
                if (matrix_height % parallel_ != 0)
                {
                    fprintf(stderr, "%s For parallel=%d we would expect the height=%d "
                                    "to be divisible by %d ??\n",
                            GetName(), parallel_, matrix_height, parallel_);
                    return false;
                }
                return true;
            }

            
            virtual void shotgun_includes_bottom_row( int x, int y, int *matrix_x, int *matrix_y ) const {
                if( x < 16 ) {
                    if ( y < 8 ) { 
                        *matrix_x = x + 16;
                        *matrix_y = y;
                    } else if ( y < 16 ) {
                        *matrix_x = x + 16;
                        *matrix_y = y;
                    } else if ( y < 24 ) {
                        *matrix_x = x + 32;
                        *matrix_y = y;
                    } else if ( y < 32 ) {
                        *matrix_x = x + 16;
                        *matrix_y = y + 8;
                    } else {
                        *matrix_x = x;
                        *matrix_y = y;
                    }
                } else if( x < 32 ) {
                    if ( y < 8 ) {
                        *matrix_x = x + 32;
                        *matrix_y = y;
                    } else if ( y < 16 ) {
                        *matrix_x = x + 16;
                        *matrix_y = y + 8;  
                    } else if ( y < 24 ) {
                        *matrix_x = x + 16;
                        *matrix_y = y + 16;  
                    } else if ( y < 32 ) {
                        *matrix_x = x + 32;
                        *matrix_y = y + 8;
                    }    
               } else if( x < 48 ) {
                    if ( y < 8 ) {
                        *matrix_x = x + 48;
                        *matrix_y = y;
                    } else if ( y < 16 ) {
                        *matrix_x = x + 8;
                        *matrix_y = y + 16;
                    } else if ( y < 24 ) {
                        *matrix_x = x + 48;
                        *matrix_y = y + 8;
                    } else if ( y < 32 ) {
                        *matrix_x = x + 48;
                        *matrix_y = y;
                    }
                } else if( x < 64 ) {
                    if ( y < 8 ) {
                        *matrix_x = x + 64;
                        *matrix_y = y;
                    } else if ( y < 16 ) {
                        *matrix_x = x + 8;
                        *matrix_y = y + 8;
                    } else if ( y < 24 ) {
                        *matrix_x = x + 16;
                        *matrix_y = y + 16;
                    } else if ( y < 32 ) {
                        *matrix_x = x + 64;
                        *matrix_y = y;
                    }
                
                // } else if( x < 80 ) {
                //     *matrix_x = x - 48;
                //     y_not_set_yet = setMatrixY( y + 1, matrix_y );
                // } else if( x < 96 ) {
                //     *matrix_x = x - 32;
                //     y_not_set_yet = setMatrixY( y + 1, matrix_y );
                // } else if( x < 112 ) {
                //     *matrix_x = x - 16;
                //     y_not_set_yet = setMatrixY( y + 1, matrix_y );
                // } else if( x < 128 ) {
                //     *matrix_x = x - 0;
                //     y_not_set_yet = setMatrixY( y + 1, matrix_y );
                // } else if( x < 144 ) {
                //     *matrix_x = x - 112;
                //     y_not_set_yet = setMatrixY( y + 2, matrix_y );
                // } else if( x < 160 ) {
                //     *matrix_x = x - 96;
                //     y_not_set_yet = setMatrixY( y + 2, matrix_y );
                // } else if( x < 176 ) {
                //     *matrix_x = x - 80;
                //     y_not_set_yet = setMatrixY( y + 2, matrix_y );
                // } else if( x < 192 ) {
                //     *matrix_x = x - 64;
                //     y_not_set_yet = setMatrixY( y + 2, matrix_y );
                // } else if( x < 208 ) {
                //     *matrix_x = x - 176;
                //     y_not_set_yet = setMatrixY( y + 3, matrix_y );
                // } else if( x < 224 ) {
                //     *matrix_x = x - 160;
                //     y_not_set_yet = setMatrixY( y + 3, matrix_y );
                // } else if( x < 240 ) {
                //     *matrix_x = x - 144;
                //     y_not_set_yet = setMatrixY( y + 3, matrix_y );
                // } else if( x < 256 ) {
                //     *matrix_x = x - 128;
                //     y_not_set_yet = setMatrixY( y + 3, matrix_y );
                // } else if( x < 272 ) {
                //     *matrix_x = x - 272;
                //     y_not_set_yet = setMatrixY( y + 4, matrix_y );
                // } else if( x < 288 ) {
                //     *matrix_x = x;
                //     y_not_set_yet = setMatrixY( y + 4, matrix_y );
                // } else if( x < 304 ) {
                //     *matrix_x = x + 16;
                //     y_not_set_yet = setMatrixY( y + 4, matrix_y );
                // } else if( x < 320 ) {
                //     *matrix_x = x + 32;
                //     y_not_set_yet = setMatrixY( y + 4, matrix_y );
                // } else if( x < 336 ) {
                //     *matrix_x = x + 336;
                //     *matrix_y = y;
                // } else if( x < 352 ) {
                //     *matrix_x = x + 352;
                //     *matrix_y = y;
                // } else if( x < 368 ) {
                //     *matrix_x = x + 368;
                //     *matrix_y = y;
                // } else if( x < 384 ) {
                //     *matrix_x = x + 384;
                //     *matrix_y = y; 
                // } else if( x < 400 ) {
                //     *matrix_x = x + 400;
                //     *matrix_y = y;
                // } else if( x < 416 ) {
                //     *matrix_x = x + 416;
                //     *matrix_y = y;
                // } else if( x < 432 ) {
                //     *matrix_x = x + 432;
                //     *matrix_y = y;
                // } else if( x < 448 ) {
                //     *matrix_x = x + 448;
                //     *matrix_y = y;
                // } else if( x < 464 ) {
                //     *matrix_x = x + 464;
                //     *matrix_y = y;
                // } else if( x < 480 ) {
                //     *matrix_x = x + 480;
                //     *matrix_y = y;
                // } else if( x < 496 ) {
                //     *matrix_x = x + 496;
                //     *matrix_y = y;
                // } else if( x < 512 ) {
                //     *matrix_x = x + 512;
                //     *matrix_y = y;
                } else {
                    *matrix_x = x;
                    *matrix_y = y;
                }
            }


            //////////////// shotgun_all_pixels_short_top_row ///////////////////////////////////////

            virtual void shotgun_all_pixels_top_row_good( int x, int y, int *matrix_x, int *matrix_y ) const {
                if( x < 16 ) {
                    if ( y < 8 ) { 
                        *matrix_x = x + 16;
                        *matrix_y = y;
                    } else if ( y < 16 ) {
                        *matrix_x = x + 32;
                        *matrix_y = y;
                    } else if ( y < 24 ) {
                        *matrix_x = x + 32;
                        *matrix_y = y;
                    } else if ( y < 32 ) {
                        *matrix_x = x + 16;
                        *matrix_y = y + 8;
                    } else {
                        *matrix_x = x;
                        *matrix_y = y;
                    }
                } else if( x < 32 ) {
                    if ( y < 8 ) {
                        *matrix_x = x + 32;
                        *matrix_y = y;
                    } else if ( y < 16 ) {
                        *matrix_x = x + 16;
                        *matrix_y = y + 8;  
                    } else if ( y < 24 ) {
                        *matrix_x = x + 16;
                        *matrix_y = y + 8;  
                    } else if ( y < 32 ) {
                        *matrix_x = x + 32;
                        *matrix_y = y + 8;
                    }    
               } else if( x < 48 ) {
                    if ( y < 8 ) {
                        *matrix_x = x + 48;
                        *matrix_y = y;
                    } else if ( y < 16 ) {
                        *matrix_x = x;
                        *matrix_y = y + 16;
                    } else if ( y < 24 ) {
                        *matrix_x = x + 32;
                        *matrix_y = y + 8;
                    } else if ( y < 32 ) {
                        *matrix_x = x + 48;
                        *matrix_y = y;
                    }
                } else if( x < 64 ) {
                    if ( y < 8 ) {
                        *matrix_x = x + 64;
                        *matrix_y = y;
                    } else if ( y < 16 ) {
                        *matrix_x = x + 16;
                        *matrix_y = y + 8;
                    } else if ( y < 24 ) {
                        *matrix_x = x + 18;
                        *matrix_y = y + 16;
                    } else if ( y < 32 ) {
                        *matrix_x = x + 32;
                        *matrix_y = y;
                    }
                
                // } else if( x < 80 ) {
                //     *matrix_x = x - 48;
                //     y_not_set_yet = setMatrixY( y + 1, matrix_y );
                // } else if( x < 96 ) {
                //     *matrix_x = x - 32;
                //     y_not_set_yet = setMatrixY( y + 1, matrix_y );
                // } else if( x < 112 ) {
                //     *matrix_x = x - 16;
                //     y_not_set_yet = setMatrixY( y + 1, matrix_y );
                // } else if( x < 128 ) {
                //     *matrix_x = x - 0;
                //     y_not_set_yet = setMatrixY( y + 1, matrix_y );
                // } else if( x < 144 ) {
                //     *matrix_x = x - 112;
                //     y_not_set_yet = setMatrixY( y + 2, matrix_y );
                // } else if( x < 160 ) {
                //     *matrix_x = x - 96;
                //     y_not_set_yet = setMatrixY( y + 2, matrix_y );
                // } else if( x < 176 ) {
                //     *matrix_x = x - 80;
                //     y_not_set_yet = setMatrixY( y + 2, matrix_y );
                // } else if( x < 192 ) {
                //     *matrix_x = x - 64;
                //     y_not_set_yet = setMatrixY( y + 2, matrix_y );
                // } else if( x < 208 ) {
                //     *matrix_x = x - 176;
                //     y_not_set_yet = setMatrixY( y + 3, matrix_y );
                // } else if( x < 224 ) {
                //     *matrix_x = x - 160;
                //     y_not_set_yet = setMatrixY( y + 3, matrix_y );
                // } else if( x < 240 ) {
                //     *matrix_x = x - 144;
                //     y_not_set_yet = setMatrixY( y + 3, matrix_y );
                // } else if( x < 256 ) {
                //     *matrix_x = x - 128;
                //     y_not_set_yet = setMatrixY( y + 3, matrix_y );
                // } else if( x < 272 ) {
                //     *matrix_x = x - 272;
                //     y_not_set_yet = setMatrixY( y + 4, matrix_y );
                // } else if( x < 288 ) {
                //     *matrix_x = x;
                //     y_not_set_yet = setMatrixY( y + 4, matrix_y );
                // } else if( x < 304 ) {
                //     *matrix_x = x + 16;
                //     y_not_set_yet = setMatrixY( y + 4, matrix_y );
                // } else if( x < 320 ) {
                //     *matrix_x = x + 32;
                //     y_not_set_yet = setMatrixY( y + 4, matrix_y );
                // } else if( x < 336 ) {
                //     *matrix_x = x + 336;
                //     *matrix_y = y;
                // } else if( x < 352 ) {
                //     *matrix_x = x + 352;
                //     *matrix_y = y;
                // } else if( x < 368 ) {
                //     *matrix_x = x + 368;
                //     *matrix_y = y;
                // } else if( x < 384 ) {
                //     *matrix_x = x + 384;
                //     *matrix_y = y; 
                // } else if( x < 400 ) {
                //     *matrix_x = x + 400;
                //     *matrix_y = y;
                // } else if( x < 416 ) {
                //     *matrix_x = x + 416;
                //     *matrix_y = y;
                // } else if( x < 432 ) {
                //     *matrix_x = x + 432;
                //     *matrix_y = y;
                // } else if( x < 448 ) {
                //     *matrix_x = x + 448;
                //     *matrix_y = y;
                // } else if( x < 464 ) {
                //     *matrix_x = x + 464;
                //     *matrix_y = y;
                // } else if( x < 480 ) {
                //     *matrix_x = x + 480;
                //     *matrix_y = y;
                // } else if( x < 496 ) {
                //     *matrix_x = x + 496;
                //     *matrix_y = y;
                // } else if( x < 512 ) {
                //     *matrix_x = x + 512;
                //     *matrix_y = y;
                } else {
                    *matrix_x = x;
                    *matrix_y = y;
                }
            }

            //////////////// got_full_screen ///////////////////////////////////////

            virtual void got_full_screen( int x, int y, int *matrix_x, int *matrix_y ) const {
                                if( x < 16 ) {
                    if ( y < 8 ) { 
                        *matrix_x = x + 16;
                        *matrix_y = y;
                    } else if ( y < 16 ) {
                        *matrix_x = x;
                        *matrix_y = y;
                    } else if ( y < 24 ) {
                        *matrix_x = x + 16;
                        *matrix_y = y;
                    } else if ( y < 32 ) {
                        *matrix_x = x + 16;
                        *matrix_y = y + 8;
                    } else {
                        *matrix_x = x;
                        *matrix_y = y;
                    }
                } else if( x < 32 ) {
                    if ( y < 8 ) {
                        *matrix_x = x + 32;
                        *matrix_y = y;
                    } else if ( y < 16 ) {
                        *matrix_x = x + 16;
                        *matrix_y = y + 8;  
                    } else if ( y < 24 ) {
                        *matrix_x = x + 16;
                        *matrix_y = y + 16;  
                    } else if ( y < 32 ) {
                        *matrix_x = x + 32;
                        *matrix_y = y + 8;
                    }    
               } else if( x < 48 ) {
                    if ( y < 8 ) {
                        *matrix_x = x + 48;
                        *matrix_y = y;
                    } else if ( y < 16 ) {
                        *matrix_x = x + 8;
                        *matrix_y = y + 16;
                    } else if ( y < 24 ) {
                        *matrix_x = x + 48;
                        *matrix_y = y + 8;
                    } else if ( y < 32 ) {
                        *matrix_x = x + 48;
                        *matrix_y = y;
                    }
                } else if( x < 64 ) {
                    if ( y < 8 ) {
                        *matrix_x = x + 64;
                        *matrix_y = y;
                    } else if ( y < 16 ) {
                        *matrix_x = x + 8;
                        *matrix_y = y + 8;
                    } else if ( y < 24 ) {
                        *matrix_x = x + 16;
                        *matrix_y = y + 16;
                    } else if ( y < 32 ) {
                        *matrix_x = x + 64;
                        *matrix_y = y;
                    }
                
                // } else if( x < 80 ) {
                //     *matrix_x = x - 48;
                //     y_not_set_yet = setMatrixY( y + 1, matrix_y );
                // } else if( x < 96 ) {
                //     *matrix_x = x - 32;
                //     y_not_set_yet = setMatrixY( y + 1, matrix_y );
                // } else if( x < 112 ) {
                //     *matrix_x = x - 16;
                //     y_not_set_yet = setMatrixY( y + 1, matrix_y );
                // } else if( x < 128 ) {
                //     *matrix_x = x - 0;
                //     y_not_set_yet = setMatrixY( y + 1, matrix_y );
                // } else if( x < 144 ) {
                //     *matrix_x = x - 112;
                //     y_not_set_yet = setMatrixY( y + 2, matrix_y );
                // } else if( x < 160 ) {
                //     *matrix_x = x - 96;
                //     y_not_set_yet = setMatrixY( y + 2, matrix_y );
                // } else if( x < 176 ) {
                //     *matrix_x = x - 80;
                //     y_not_set_yet = setMatrixY( y + 2, matrix_y );
                // } else if( x < 192 ) {
                //     *matrix_x = x - 64;
                //     y_not_set_yet = setMatrixY( y + 2, matrix_y );
                // } else if( x < 208 ) {
                //     *matrix_x = x - 176;
                //     y_not_set_yet = setMatrixY( y + 3, matrix_y );
                // } else if( x < 224 ) {
                //     *matrix_x = x - 160;
                //     y_not_set_yet = setMatrixY( y + 3, matrix_y );
                // } else if( x < 240 ) {
                //     *matrix_x = x - 144;
                //     y_not_set_yet = setMatrixY( y + 3, matrix_y );
                // } else if( x < 256 ) {
                //     *matrix_x = x - 128;
                //     y_not_set_yet = setMatrixY( y + 3, matrix_y );
                // } else if( x < 272 ) {
                //     *matrix_x = x - 272;
                //     y_not_set_yet = setMatrixY( y + 4, matrix_y );
                // } else if( x < 288 ) {
                //     *matrix_x = x;
                //     y_not_set_yet = setMatrixY( y + 4, matrix_y );
                // } else if( x < 304 ) {
                //     *matrix_x = x + 16;
                //     y_not_set_yet = setMatrixY( y + 4, matrix_y );
                // } else if( x < 320 ) {
                //     *matrix_x = x + 32;
                //     y_not_set_yet = setMatrixY( y + 4, matrix_y );
                // } else if( x < 336 ) {
                //     *matrix_x = x + 336;
                //     *matrix_y = y;
                // } else if( x < 352 ) {
                //     *matrix_x = x + 352;
                //     *matrix_y = y;
                // } else if( x < 368 ) {
                //     *matrix_x = x + 368;
                //     *matrix_y = y;
                // } else if( x < 384 ) {
                //     *matrix_x = x + 384;
                //     *matrix_y = y; 
                // } else if( x < 400 ) {
                //     *matrix_x = x + 400;
                //     *matrix_y = y;
                // } else if( x < 416 ) {
                //     *matrix_x = x + 416;
                //     *matrix_y = y;
                // } else if( x < 432 ) {
                //     *matrix_x = x + 432;
                //     *matrix_y = y;
                // } else if( x < 448 ) {
                //     *matrix_x = x + 448;
                //     *matrix_y = y;
                // } else if( x < 464 ) {
                //     *matrix_x = x + 464;
                //     *matrix_y = y;
                // } else if( x < 480 ) {
                //     *matrix_x = x + 480;
                //     *matrix_y = y;
                // } else if( x < 496 ) {
                //     *matrix_x = x + 496;
                //     *matrix_y = y;
                // } else if( x < 512 ) {
                //     *matrix_x = x + 512;
                //     *matrix_y = y;
                } else {
                    *matrix_x = x;
                    *matrix_y = y;
                }
            }

             //////////////// current_experiment ///////////////////////////////////////

            virtual void current_experiment( int x, int y, int *matrix_x, int *matrix_y ) const {
                if( x < 16 ) {
                    if ( y < 8 ) { 
                        *matrix_x = x + 16;
                        *matrix_y = y;
                    } else if ( y < 16 ) {
                        *matrix_x = x + 16;
                        *matrix_y = y;
                    } else if ( y < 24 ) {
                        *matrix_x = x + 16;
                        *matrix_y = y;
                    } else if ( y < 32 ) {
                        *matrix_x = x + 16;
                        *matrix_y = y;
                    } else {
                        *matrix_x = x;
                        *matrix_y = y;
                    }
                } else if( x < 32 ) {
                    if ( y < 8 ) {
                        *matrix_x = x + 32;
                        *matrix_y = y;
                    } else if ( y < 16 ) {
                        *matrix_x = x + 32;
                        *matrix_y = y;  
                    } else if ( y < 24 ) {
                        *matrix_x = x + 32;
                        *matrix_y = y;  
                    } else if ( y < 32 ) {
                        *matrix_x = x + 32;
                        *matrix_y = y;
                    }    
               } else if( x < 48 ) {
                    if ( y < 8 ) {
                        *matrix_x = x + 48;
                        *matrix_y = y;
                    } else if ( y < 16 ) {
                        *matrix_x = x + 48;
                        *matrix_y = y;
                    } else if ( y < 24 ) {
                        *matrix_x = x + 48;
                        *matrix_y = y;
                    } else if ( y < 32 ) {
                        *matrix_x = x + 48;
                        *matrix_y = y;
                    }
                } else if( x < 64 ) {
                    if ( y < 8 ) {
                        *matrix_x = x + 64;
                        *matrix_y = y;
                    } else if ( y < 16 ) {
                        *matrix_x = x + 64;
                        *matrix_y = y;
                    } else if ( y < 24 ) {
                        *matrix_x = x + 64;
                        *matrix_y = y;
                    } else if ( y < 32 ) {
                        *matrix_x = x + 64;
                        *matrix_y = y;
                    }
                } else {
                    *matrix_x = x;
                    *matrix_y = y;
                }
            }

            // virtual void MapVisibleToMatrix(int matrix_width, int matrix_height,
            //                                 int x, int y,
            //                                 int *matrix_x, int *matrix_y) const {
            //     if( x < 16 ) {
            //         if ( y < 8 ) { 
            //             *matrix_x = x + 16;
            //             *matrix_y = y;
            //         } else if ( y < 16 ) {
            //             *matrix_x = x + 16;
            //             *matrix_y = y;
            //         } else if ( y < 24 ) {
            //             *matrix_x = x + 16;
            //             *matrix_y = y;
            //         } else if ( y < 32 ) {
            //             *matrix_x = x + 16;
            //             *matrix_y = y;
            //         } else {
            //             *matrix_x = x;
            //             *matrix_y = y;
            //         }
            //     } else if( x < 32 ) {
            //         if ( y < 8 ) {
            //             *matrix_x = x + 32;
            //             *matrix_y = y;
            //         } else if ( y < 16 ) {
            //             *matrix_x = x + 32;
            //             *matrix_y = y;  
            //         } else if ( y < 24 ) {
            //             *matrix_x = x + 32;
            //             *matrix_y = y;  
            //         } else if ( y < 32 ) {
            //             *matrix_x = x + 32;
            //             *matrix_y = y;
            //         }    
            //    } else if( x < 48 ) {
            //         if ( y < 8 ) {
            //             *matrix_x = x + 48;
            //             *matrix_y = y;
            //         } else if ( y < 16 ) {
            //             *matrix_x = x + 48;
            //             *matrix_y = y;
            //         } else if ( y < 24 ) {
            //             *matrix_x = x + 48;
            //             *matrix_y = y;
            //         } else if ( y < 32 ) {
            //             *matrix_x = x + 48;
            //             *matrix_y = y;
            //         }
            //     } else if( x < 64 ) {
            //         if ( y < 8 ) {
            //             *matrix_x = x + 64;
            //             *matrix_y = y;
            //         } else if ( y < 16 ) {
            //             *matrix_x = x + 64;
            //             *matrix_y = y;
            //         } else if ( y < 24 ) {
            //             *matrix_x = x + 64;
            //             *matrix_y = y;
            //         } else if ( y < 32 ) {
            //             *matrix_x = x + 64;
            //             *matrix_y = y;
            //         }
            //     } else {
            //         *matrix_x = x;
            //         *matrix_y = y;
            //     }
            // }

        void MapVisibleToMatrix(int matrix_width, int matrix_height,
                                            int x, int y,
                                            int *matrix_x, int *matrix_y) const {
                    int offset;
            if (x < 16) {
                offset = 16;
            } else if (x < 32) {
                offset = 32;
            } else if (x < 48) {
                offset = 48;
            } else if (x < 64) {
                offset = 64;
            } else {
                offset = 0;
            }
            if (y < 8 || (y < 16 && offset != 0) || (y < 24 && offset != 0) || (y < 32 && offset != 0)) {
                *matrix_x = x + offset;
                *matrix_y = y;
            } else {
                *matrix_x = x;
                *matrix_y = y;
            }
        }             
        
        private:
            int parallel_;
 };

 /*
   saturday  feb 11, 2023
    works with chain=4, "U-mapper;Rotate:180"
 */


        // case 180:
        //             *matrix_x = matrix_width - x - 1;
        //             *matrix_y = matrix_height - y - 1;

        // This function maps visible coordinates to matrix coordinates
        void MapVisibleToMatrix(int matrix_width, int matrix_height, 
                                int x, int y, 
                                int *matrix_x, int *matrix_y) const {
            // Calculate panel height
            const int panel_height = matrix_height / parallel_;
            // Calculate visible width 
            const int visible_width = (matrix_width / 64) * 32;
            // Calculate slab height
            const int slab_height = 2 * panel_height;
            
            // Calculate base y
            const int base_y = (y / slab_height) * panel_height;
            // Update y
            y %= slab_height;

            // Check which side the coordinates are on 
            if (y < panel_height) { 
                // On top panel, invert x and y
                x = matrix_width  - x - 1;
                y = matrix_height - y - 1; // lets try matrix_height here instead of panel_height
            } else {
                // On the bottom panel, invert x
                x = visible_width - x - 1;

                // Invert y
                y = slab_height - y - 1; }
         }    




         // this works until > 64
                 /*
            This function maps visible coordinates to matrix coordinates.
            
            panel_height has been removed from the code, as _parallel_ is always 1
            remember to put it pack in for _parallel values greater than 1

        */
        void MapVisibleToMatrix(int matrix_width, int matrix_height, 
                                int x, int y, 
                                int *matrix_x, int *matrix_y) const {
            
            // matrix_width = 128; // lets start making constants
            // Calculate visible width 
            // const int visible_width = ( 128 / 64 ) * 32;
            // matrix_height = 32; // lets start making constants
            // const int slab_height = 64;

            // Calculate slab height
            // const int slab_height = 2 * 32;
                    
            // Calculate base y
            // const int base_y = ( y / 64 ) * 32;
            // Update y
            // y %= 64;
            // base_y = 0 always

            // Check which side the coordinates are on 
            if ( y < 32 ) {
                x = x;
                y = y;
            } else if ( y < 64 ) {
                x = x + 64;
                y = y - 32;
            } else if ( y < 96 ) {
                x = x;
                y = y;
            } else {
                x = x + 64;
                y = y - 32;
            }

            // Update matrix coordinates
            *matrix_x = x;
            *matrix_y = /* base_y */ 0 + y;

            // printf( "x: %d, y: %d, matrix_x: %d, matrix_y: %d\n", x, y, *matrix_x, *matrix_y );
        }