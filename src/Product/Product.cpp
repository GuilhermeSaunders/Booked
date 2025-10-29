#include "product.h"

Product::Product(float rentValue,
         std::string id,
         std::string owner,
         std::string name,
         std::string description,  std::string idiom,
         std::string recommendedAge,  std::string genre, bool isRented)

    :   rentValue(rentValue),
        id(std::move(id)),
        owner(std::move(owner)),
        name(std::move(name)),
        description(std::move(description)),
        idiom(std::move(idiom)),
        recommendedAge(std::move(recommendedAge)),
        genre(std::move(genre)),
        isRented(isRented){}



Product::~Product() = default;


void Product::setRentValue(float rentvalue){this->rentValue = rentvalue;}
float Product::getRentValue() const {return rentValue;}


void Product::setOwner(std::string owner){this-> owner = std::move(owner);}
const std::string& Product::getOwner() const {return owner;}



void Product::setGenre(std::string genre){this->genre = std::move(genre);}
const std::string& Product::getGenre() const {return genre;}

void Product::setName(std::string name){this->name = std::move(name);}
const std::string& Product::getName() const {return name;}

void Product::setDescription(std::string description){this->description = std::move(description);}
const std::string& Product::getDescription() const {return description;}

void Product::setIdiom(std::string idiom){this->idiom = std::move(idiom);}
const std::string& Product::getIdiom() const {return idiom;}

void Product::setRecommendedAge(std::string recommendedAge){this->recommendedAge = std::move(recommendedAge);}
const std::string& Product::getRecommendedAge() const {return recommendedAge;}

void Product::setId(std::string id){this->id = std::move(id);}
const std::string& Product::getId() const {return id;}


void Product::setIsRented(bool isRented){this->isRented = isRented;}
bool Product::getIsRented() const {return isRented;}