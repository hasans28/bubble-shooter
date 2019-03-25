typedef struct bubble 
{
    int id;                  // array index
    int x_loc; 
    int y_loc;               // location of center
    short int colour;        // colour of bubble
    struct bubble* next;            // next connection to same coloured bubble
}