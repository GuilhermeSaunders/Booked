#include "product.h"

void product::setRentValue(float rentvalue){this->rentValue = rentvalue;}
const float product::getRentValue() const {return rentValue;}

void product::setConsideredValue(float consideredValue){this->consideredValue = consideredValue;}
const float product::getConsideredValue() const {return consideredValue;}

void product::setDuration(float duration){this->duration = duration;}
const float product::getDuration() const {return duration;}

void product::setProductType(string productType){this->productType = move(productType);}
const string& product::getProductType() const {return productType;}

void product::setOwner(string owner){this-> owner = move(owner);}
const string& product::getOwner() const {return owner;}

void product::setGenre(string genre){this->genre = move(genre);}
const string& product::getGenre() const {return genre;}

void product::setOwnersConsiderations(string ownersConsiderations){this->ownersConsiderations = move(ownersConsiderations);}
const string& product::getOwnersConsiderations() const {return ownersConsiderations;}

void product::setName(string name){this->name = move(name);}
const string& product::getName() const {return name;}

void product::setDescription(string description){this->description = move(description);}
const string& product::getDescription() const {return description;}

void product::setIdiom(string idiom){this->idiom = move(idiom);}
const string& product::getIdiom() const {return idiom;}

void product::setRentalPeriod(int rentalPeriod){this->rentalPeriod = rentalPeriod;}
int product::getRentalPeriod() const {return rentalPeriod;}

void product::setRecommendedAge(int recommendedAge){this->recommendedAge = recommendedAge;}
int product::getRecommendedAge() const {return recommendedAge;}

void product::setId(int id){this->id= id;}
int product::getId() const {return id;}