#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <cmath>  
#include <vector> 
#include <bitset>
#include <sys/time.h>
using namespace std;

#define fieldNO 17 // no. of columns in the data file
#define gridx_base 0.015 // for 500 metre
#define gridy_base 0.010 // for 500 metre
#define range_sy 35.0 // coordinate of New Zealand
#define range_ey 38.0
#define range_sx 173.5
#define range_ex 176.5

/* Type of the property */
#define T_COMMERCIAL 1
#define T_INDUSTRIAL 2
#define T_RESIDENTIAL 4
#define T_APARTMENT 8
#define T_HOUSE 16
#define T_RETAIL 32
#define T_OFFICE 64
#define T_EDUCATION 128
#define T_PUBLICHSE 256

class property {        // The class property
    int ID;                             // Attribute (int property ID)
    string title;                      // Attribute (string valuation reference)
    string legal_des;                   // Attribute (string legal description)
    float latitude;                     // Attribute (float latitude)
    float longitude;                    // Attribute (float longitude)
    int land_val;                       // Attribute (int land value)
    int impr_val;                       // Attribute (int improvement value)
    unsigned short int type;            // Attribute (int type of property)
    int x;                              // Attribute (int x-coordinate of region)
    int y;                              // Attribute (int y-coordinate of region)
    unsigned short int no_comm;         // Attribute (int number of commercial buildings)
    unsigned short int no_ind;          // Attribute (int number of industrial buildings)
    unsigned short int no_res;          // Attribute (int number of residential buildings)
    unsigned short int no_apart;        // Attribute (int number of apartments)
    unsigned short int no_hou;          // Attribute (int number of houses)
    unsigned short int no_ret;          // Attribute (int number of retail buildings)
    unsigned short int no_off;          // Attribute (int number of office buildings)
    unsigned short int no_edu;          // Attribute (int number of education buildings)
    unsigned short int no_pubhse;       // Attribute (int number of public houses)

    public:
        void setvalue(int pID, string ptitle, string plegal_des, float platitude, float plongitude, int pland_val, int pimpr_val, unsigned short int ptype) { // set the values
            ID = pID;
            title = ptitle;
            legal_des = plegal_des;
            latitude = platitude;
            longitude = plongitude;
            land_val = pland_val;
            impr_val = pimpr_val;
            type = ptype;
            no_comm = 0;
            no_ind = 0;
            no_res = 0;
            no_apart = 0;
            no_hou = 0;
            no_ret = 0;
            no_off = 0;
            no_edu = 0;
            no_pubhse = 0;
        }
        void setregion(int region_x, int region_y) {
            x = region_x;
            y = region_y;
        }
        int outID() {
            return ID;
        }
        string outName() {
            return title;
        }
        float outLat() {
            return latitude;
        }
        float outLong() {
            return longitude ;
        }
        int outLand_val() {
            return land_val;
        }
        int outImpr_val() {
            return impr_val;
        }
        unsigned short int outType() {
            return type;
        }
        int outx() {
            return x;
        }        
        int outy() {
            return y;
        }
        int outNo_comm() {
            return no_comm;
        }
        int outNo_ind() {
            return no_ind;
        }
        int outNo_res() {
            return no_res;
        }
        int outNo_apart() {
            return no_apart;
        }
        int outNo_hou() {
            return no_hou;
        }
        int outNo_ret() {
            return no_ret;
        }
        int outNo_off() {
            return no_off;
        }
        int outNo_edu() {
            return no_edu;
        }
        int outNo_pubhse() {
            return no_pubhse;
        }
        void add_comm() {
            no_comm = no_comm + 1;
        }
        void add_ind() {
            no_ind = no_ind + 1;
        }
        void add_res() {
            no_res = no_res + 1;
        }
        void add_apart() {
            no_apart = no_apart + 1;
        }
        void add_hou() {
            no_hou = no_hou + 1;
        }
        void add_ret() {
            no_ret = no_ret + 1;
        }
        void add_off() {
            no_off = no_off + 1;
        }
        void add_edu() {
            no_edu = no_edu + 1;
        }
        void add_pubhse() {
            no_pubhse = no_pubhse + 1;
        }
};

class cluster {
    vector<property*> listofProperty;
    int id_x;
    int id_y;
    public:
        void setvalue(int pid_x, int pid_y) { // set the values
            id_x = pid_x;
            id_y = pid_y;
        }
        void add(property* pproperty) { // add a property to the vector
            listofProperty.push_back(pproperty);
        }
        void create() {
            listofProperty = {};
        }
        vector<property*> list() {
            return listofProperty;
        }
        property* view(int index) {
            return listofProperty.at(index);
        }
        int cap() {
            return listofProperty.size();
        }
};

float calDistance(float lat1, float long1, float lat2, float long2, float coeffecient) {
       
    float dlat1=lat1*coeffecient;
    float dlong1=long1*coeffecient;
    float dlat2=lat2*coeffecient;
    float dlong2=long2*coeffecient;

    float dLong=dlong1-dlong2;
    float dLat=dlat1-dlat2;

    float aHarv= pow(sin(dLat/2.0),2.0)+cos(dlat1)*cos(dlat2)*pow(sin(dLong/2),2);
    float cHarv=2*atan2(sqrt(aHarv),sqrt(1.0-aHarv));
        //earth's radius from wikipedia varies between 6,356.750 km — 6,378.135 km (˜3,949.901 — 3,963.189 miles)
        //The IUGG value for the equatorial radius of the Earth is 6378.137 km (3963.19 mile)
    const float earth=6378.13;
    float distance=earth*cHarv;
    return distance;
}

void split (string str, char seperator, string* indata) {  
    int currIndex = 0;
    int i = 0;  
    int startIndex = 0; 
    int endIndex = 0;  
    while (i <= str.size() && currIndex < fieldNO) {
        if (str[i] == seperator || i == str.size()) {  
            endIndex = i;  
            string subStr = "";  
            subStr.append(str, startIndex, endIndex - startIndex);  
            indata[currIndex] = subStr;  
            currIndex += 1;  
            startIndex = endIndex + 1;  
        }  
        i++;  
    }     
}  

int main(int argc, char *argv[]) {

    string rawfile1 = "./rawfiles/dataset.csv";
    ifstream buildingfile(rawfile1);
    string commandline = "Command line: Test <option: 1 for 500m; 2 for 1000m; 3 for 1500m>\0";
    string line;
    string datain[fieldNO];
    int i, j, k;
    property *building;
    cluster **region;
    int buildingNo;
    int option, buildingtype;
    int boundx, boundy;
    float coeffecient;
    struct timespec start, end;
    float grid_x, grid_y, grid_factor;
    unsigned short int unique = 0;

/* getting input arguments */
    if (argc < 2) {
        cout << commandline << endl;
        return 1;
    }
    else
    {
        option = stoi(argv[1]);
        if ((option < 1) || (option > 4)) {
            cout << commandline << endl;
            return 1;            
        }
        else {
            option = option * 500;
        }
        if (argc > 3) {
            unique = 1;
        }
        if (argc == 3) {
            grid_factor = stoi(argv[2]);
            if ((grid_factor < 1) || (grid_factor > 20)) {
                cout << commandline << endl;
                return 1;  
            }
        }
        else {
            grid_factor = 1;
        }
    }
    cout << "Option:" << option << endl;
    grid_x = gridx_base * grid_factor; 
    grid_y = gridy_base * grid_factor;

    coeffecient = 4.0*atan(1.0)/180;

    cout << "Creating object for properties" << endl;
    boundx = ceil((range_ex - range_sx)/grid_x);
    boundy = ceil((range_ey - range_sy)/grid_y);

    building = (property*)malloc(sizeof(property)*1000000);
 
    cout << "Creating cluster (region size:" << option << "m)" << endl;
    region = (cluster**)malloc(sizeof(cluster*)*boundx);
    for (i = 0; i < boundx; i++) {
        region[i] = (cluster*)malloc(sizeof(cluster)*boundy);
        for (j = 0; j < boundy; j++) {
            region[i][j].create();
            region[i][j].setvalue(i, j);
        }
    }


     clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

    cout << "Reading file: " << rawfile1 << endl;
// Use a while loop together with the getline() function to read the file line by line
    getline (buildingfile, line);

    i = 0;
    while (getline (buildingfile, line)) {
        int region_x, region_y;
        unsigned short int ptype;
        string ptitle, plegal_des;
        int pland_val, pimpr_val;

        split (line, ',', datain);

        if ((datain[10].length() != 0) && (datain[11].length() != 0)) {
            ptype = stoi(datain[0]) * T_APARTMENT + stoi(datain[1]) * T_COMMERCIAL + stoi(datain[2]) * T_EDUCATION + stoi(datain[4]) * T_HOUSE + 
                    stoi(datain[5]) * T_INDUSTRIAL + stoi(datain[6]) * T_OFFICE + stoi(datain[7]) * T_PUBLICHSE + stoi(datain[8]) * T_RESIDENTIAL + stoi(datain[9]) * T_RETAIL;
            if (datain[12].length() != 0)
                ptitle = datain[12];
            else
                ptitle = "Title_Unknown";
            pland_val = round(stof(datain[15])/1000);
            pimpr_val = round(stof(datain[13])/1000);
            plegal_des = datain[16];

            building[i].setvalue(i, ptitle, plegal_des, abs(stof(datain[10])), abs(stof(datain[11])), pland_val, pimpr_val, ptype);
            if ((building[i].outLong() > range_ex) || (building[i].outLat() > range_ey)) {
                cout << building[i].outLong() << endl;
                cout << building[i].outLat() << endl;         
            }
            region_x = floor((building[i].outLong()-range_sx)/grid_x);
            region_y = floor((building[i].outLat()-range_sy)/grid_y);
            building[i].setregion(region_x, region_y);
            region[region_x][region_y].add(&(building[i]));
            i++;
        }
    }   
    buildingNo = i;
    cout << "Total number of properties read:" << buildingNo << endl;

/*
    for (i = 0; i < boundx; i++) {
        for (j = 0; j < boundy; j++) {
            if (region[i][j].cap() > 0) {
                for (k = 0; k < region[i][j].cap(); k++) {
                    property *p1;

                    p1 = region[i][j].view(k);
                }
            }
        }
    }
*/
    cout << "Counting:Title:Type:Land Value:Improvement Value:Total:Commercial buildings:Industrial Buildings:Residential buildings:Apartments:Houses:Retails:Offices:Educational:Buildings:State Houses" << endl;
    if (unique == 1) {
        cout << "Information for property:" << argv[3] << endl;
    }
    for (i = 0; i < buildingNo; i++) {
        int x, y;
        int pubCnt = 0;
        int neigCnt = 0;
        int xs, xe, ys, ye;
        int nb_cluster;

        if (unique == 1) {
//            cout << building[i].outName().c_str() << argv[3] << endl;
//            if (strcmp(building[i].outName().c_str(),argv[3]) != 0) {
            if (strcmp(building[i].outName().c_str(),argv[3]) != 0) {
                continue;
            }
        }

        nb_cluster = option / 500;
        x = building[i].outx();
        y = building[i].outy();
        xs = x - nb_cluster;
        if (xs < 0) {
            xs = 0;
        }
        ys = y - nb_cluster;
        if (ys < 0) {
            ys = 0;
        }
        xe = x + nb_cluster;
        if (xe >= boundx) {
            xe = boundx - 1;
        }
        ye = y + nb_cluster;
        if (ye >= boundy) {
            ye =  boundy - 1;
        }
        for (j = xs; j <= xe; j++) {
            for (k = ys; k <= ye; k++) {
                for (property* p2 : region[j][k].list()) {
                    float dist, tdist;

                    tdist = option/1000.0;
                    dist = calDistance(building[i].outLat(), building[i].outLong(), (*p2).outLat(), (*p2).outLong(), coeffecient);
                    if (dist < tdist) {
                        neigCnt = neigCnt + 1;
                        if (((*p2).outType() & T_COMMERCIAL) > 0) {
                            building[i].add_comm();
                            if (unique == 1) {
                                cout << "Commercial building:";
                            }
                        }
                        if (((*p2).outType() & T_INDUSTRIAL) > 0) {
                            building[i].add_ind();
                            if (unique == 1) {
                                cout << "Industrial building:";
                            }
                        }
                        if (((*p2).outType() & T_RESIDENTIAL) > 0) {
                            building[i].add_res();
                            if (unique == 1) {
                                cout << "Residential building:";
                            }
                        }
                        if (((*p2).outType() & T_APARTMENT) > 0) {
                            building[i].add_apart();
                            if (unique == 1) {
                                cout << "Apartment:";
                            }
                        }
                        if (((*p2).outType() & T_HOUSE) > 0) {
                            building[i].add_hou();
                            if (unique == 1) {
                                cout << "House:";
                            }
                        }
                        if (((*p2).outType() & T_RETAIL) > 0) {
                            building[i].add_ret();
                            if (unique == 1) {
                                cout << "Retails:";
                            }
                        }
                        if (((*p2).outType() & T_OFFICE) > 0) {
                            building[i].add_off();
                            if (unique == 1) {
                                cout << "Office:";
                            }
                        }
                        if (((*p2).outType() & T_EDUCATION) > 0) {
                            building[i].add_edu();
                            if (unique == 1) {
                                cout << "Education building:";
                            }
                        }
                        if (((*p2).outType() & T_PUBLICHSE) > 0) {
                            building[i].add_pubhse();
                            if (unique == 1) {
                                cout << "Public House:";
                            }
                        }
                        if (unique == 1) {
                            cout << (*p2).outID()  << ':';
                            cout << (*p2).outName() << ':';
                            cout << (*p2).outLand_val() << "k:";
                            cout << (*p2).outImpr_val() << "k" << endl;                            
                        } 
                    }
                }
            }
        }
        cout << building[i].outID()  << ':';
        cout << building[i].outName() << ':';
        cout << bitset<12>(building[i].outType()) << ':';
        cout << building[i].outLand_val() << "k:";
        cout << building[i].outImpr_val() << "k:";
        cout << neigCnt << ':';
        cout << building[i].outNo_comm() << ':';
        cout << building[i].outNo_ind() << ':';
        cout << building[i].outNo_res() << ':';
        cout << building[i].outNo_apart() << ':';
        cout << building[i].outNo_hou() << ':';
        cout << building[i].outNo_ret() << ':';
        cout << building[i].outNo_off() << ':';
        cout << building[i].outNo_edu() << ':';
        cout << building[i].outNo_pubhse() << endl;
    }

    free(building);
    for (i = 0; i < boundx; i++) {
        free(region[i]);
    }
    free(region);
    
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    
    double time_taken;
    time_taken = (end.tv_sec - start.tv_sec) * 1e9;
    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
    cout << "Time taken by function: " << ceil(time_taken) << " seconds" << endl;

    return 0;
}