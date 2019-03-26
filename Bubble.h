typedef struct bubble 
{
    int id;                             // array index
    int x_loc; 
    int y_loc;                              // location of center
    short int colour;                   // colour of bubble
    bool visited;
    struct bubble* next[8];            // next connection to same coloured bubble
}