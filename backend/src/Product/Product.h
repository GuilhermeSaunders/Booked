#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
using namespace std;

class Product {
    private:
        float rentValue, consideredValue, duration; //duration => avarege duration of time the product takes to be consumed

        //retirar productType => sem utilidade
        string productType, owner, genre, ownersConsiderations, name, description, idiom, recommendedAge;

        int rentalPeriod, id;
    
    public:
        virtual void showInfor() const = 0;
        virtual void dealTerms() const = 0;

        void setRentValue(float rentvalue); 
        const float getRentValue() const;

        // consideredValue => if you would seal the product, at wich price would you sell??
        // it will be the guarantee value 
        void setConsideredValue(float consideredValue);
        const float getConsideredValue() const;

        void setDuration(float duration);
        const float getDuration() const;

        void setProductType(string productType);
        const string& getProductType() const;

        void setOwner(string owner);
        const string& getOwner() const;

        void setGenre(string genre);
        const string& getGenre() const;

        void setOwnersConsiderations(string ownersConsiderations = "No written consideration by the owner");
        const string& getOwnersConsiderations() const;

        void setName(string name);
        const string& getName() const;

        void setDescription(string description = "No description provided, but you can search the title on the internet and you will, probably, find something");
        const string& getDescription() const;

        void setIdiom(string idiom);
        const string& getIdiom() const;

        void setRecommendedAge(string recommendedAge = "Age recommendation not provided");
        const string& getRecommendedAge() const;

        void setRentalPeriod(int rentalPeriod);
        int getRentalPeriod() const;

        void setId(int id);
        int getId() const;
};

#endif