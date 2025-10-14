#include "Product.h"

void Product::setRentValue(float rentvalue){this->rentValue = rentvalue;}
const float Product::getRentValue() const {return rentValue;}

void Product::setConsideredValue(float consideredValue){this->consideredValue = consideredValue;}
const float Product::getConsideredValue() const {return consideredValue;}

void Product::setDuration(float duration){this->duration = duration;}
const float Product::getDuration() const {return duration;}

void Product::setProductType(string ProductType){this->productType = move(productType);}
const string& Product::getProductType() const {return productType;}

void Product::setOwner(string owner){this-> owner = move(owner);}
const string& Product::getOwner() const {return owner;}

void Product::setGenre(string genre){this->genre = move(genre);}
const string& Product::getGenre() const {return genre;}

void Product::setOwnersConsiderations(string ownersConsiderations){this->ownersConsiderations = move(ownersConsiderations);}
const string& Product::getOwnersConsiderations() const {return ownersConsiderations;}

void Product::setName(string name){this->name = move(name);}
const string& Product::getName() const {return name;}

void Product::setDescription(string description){this->description = move(description);}
const string& Product::getDescription() const {return description;}

void Product::setIdiom(string idiom){this->idiom = move(idiom);}
const string& Product::getIdiom() const {return idiom;}

void Product::setRecommendedAge(string recommendedAge){this->recommendedAge = move(recommendedAge);}
const string& Product::getRecommendedAge() const {return recommendedAge;}

void Product::setRentalPeriod(int rentalPeriod){this->rentalPeriod = rentalPeriod;}
int Product::getRentalPeriod() const {return rentalPeriod;}

void Product::setId(int id){this->id= id;}
int Product::getId() const {return id;}