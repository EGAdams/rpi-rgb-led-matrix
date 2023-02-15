

// bool isTopCheck( int y, int panel_rows_ ) const {
//     // printf( "returning the top check: y < panel_rows_ / 4: %d\n", y < panel_rows_ / 4 );
//     // return y < panel_rows_ / 4;
//     return ( y % ( panel_rows_ / 2 )) < panel_rows_ / 4;
// }

function isTopCheck( y ) {
    // printf( "returning the top check: y < panel_rows_ / 4: %d\n", y < panel_rows_ / 4 );
    // return y < panel_rows_ / 4;
    let y_mod_16 = ( y % 16 );
    console.log ( "y: " + y + "  y_mod_16: " + y_mod_16 );  
    let panel_rows_divided_by_four = 8 ;
    console.log ( "panel_rows_divided_by_four: " + panel_rows_divided_by_four );
    
    return y_mod_16 < panel_rows_divided_by_four;
}

for ( let i = 0; i < 32; i++ ) { console.log ( isTopCheck( i )); }


