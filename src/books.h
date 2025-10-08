#include <string>

using namespace std;

class Books{
    
    private:

        string genre, title, author, description, owner;
        float value;
        int id, pages;
        bool sellOrRent, isRented = false;
        /* sellOrRnet: true => for rent, otherwise => for sale 
    isRented: if the books is avaible for rent, we need to know if its is rented or not */

    public:
        //methods for each attribute
        void setGenre(string genre);
        string getGenre();

        void setTitle(string title);
        string getTitle();

        void setAuthor(string author);
        string  getAuthor();

        void setDescription(string description);
        string getDescription();

        void setOwner(string owner);
        string getOwner();

        void setValue(float value);
        float getValue();

        void setId(int id);
        int getId();

        void setPages(int pages);
        int getPages();

        void setStatus(bool isRented);
        bool getStatus();
};

