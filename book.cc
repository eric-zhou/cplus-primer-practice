#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

class ItemBook {
public:
   ItemBook(const std::string &book = "", double sales_price = 0.0):
      isbn(book), price(sales_price) { 
        std::cout << "call construtor of ItemBook"  << std::endl;
      }

   ItemBook(const ItemBook &book) {
       std::cout << "call copy construtor of ItemBook"  << std::endl;
       isbn = book.isbn;
       price = book.price;
   }

   std::string getBook() const {
      return isbn;
   }

   virtual double getPrice(std::size_t n) const {
      std::cout << "call getPrice of ItemBook"  << std::endl;
      return n * price;
   }

   virtual ItemBook* clone() const {
       return new ItemBook(*this);
   }

   void printPrice(int count) {
      std::cout << getBook() << std::endl;
      std::cout << getPrice(count) << std::endl;
   }

   virtual void show() {
      std::cout << "isbn:\t" << isbn << std::endl;
      std::cout << "price\t" << price << std::endl;
   }
   //~ItemBook() {
       //std::cout << "call destrutor of ItemBook"  << std::endl;
   //}

   virtual ~ItemBook() {
       std::cout << "call destrutor of ItemBook"  << std::endl;
   }
private:
   std::string isbn;

protected:
   double price;
};

class DiscItem: public ItemBook {
public:
    DiscItem(const std::string &book = "", double sales_price = 0.0, 
        double qty = 0, double discRate = 0) :
        ItemBook(book, sales_price), quantity(qty), discount(discRate) {
    }

    //pure virtual function
    double getPrice(std::size_t) const = 0;
private:
    std::size_t quantity;
    double discount;
};

class BulkBook: public ItemBook {
public:
   BulkBook(const std::string &book = "", double sales_price = 0.0, double qty = 0, 
      double discRate = 0) : 
      ItemBook(book, sales_price), minQty(qty), discount(discRate) {
        std::cout << "call construtor of BulkBook"  << std::endl;
   }

   BulkBook(const BulkBook &book): ItemBook(book) {
       std::cout << "call copy construtor of BulkBook"  << std::endl;
   }

   double getPrice(std::size_t n) const {
      std::cout << "call getPrice of BulkBook"  << std::endl;
      if (n >= minQty)
        return (1 - discount) * price * n;
      else
        return ItemBook::getPrice(n);
   }

   virtual BulkBook* clone() const {
       return new BulkBook(*this);
   }

   virtual void show() {
        ItemBook::show();  
        std::cout << "minQty:\t" << minQty << std::endl;
        std::cout << "discount:\t" << discount << std::endl;
   }

   ~BulkBook() {
       std::cout << "call destrutor of BulkBook"  << std::endl;
   }

private:
   std::size_t minQty;
   double discount;
};

//test
struct Base{
    Base(int val): id(val) { }
protected:
    int id;
};

struct C1: public Base{
    //C1(int val): Base(val), id(val) { }
    //C1(int val):Base(val) { 
        //id = val;
    //}
    int hello() {
        id = 0;
    }
};

void printPrice(ItemBook book)
{
    book.printPrice(20);
}

void printPrice1(ItemBook *book)
{
    book->printPrice(20);
}

int main(int argc, const char *argv[])
{
   ItemBook base("itemBook", 15);
   base.printPrice(20);
   base.show();
   ItemBook base1("itemBook", 15);
   base.printPrice(30);

   BulkBook bulk("bulkBook", 15, 20, 0.1);
   bulk.printPrice(20);
   bulk.show();

   std::cout << std::endl;
   std::cout << "vector begin" << std::endl;
   std::vector<ItemBook> basket;
   basket.push_back(base);
   basket.push_back(bulk);
   basket.push_back(base1);
   std::cout << "begin for each" << std::endl;
   std::for_each(basket.begin(), basket.end(), printPrice);

   std::cout << std::endl;
   //ItemBook *item = new BulkBook;
   //delete item;

   std::cout << "refernce vector begin" << std::endl;
   std::vector<ItemBook*> refBasket;
   refBasket.push_back(&base);
   refBasket.push_back(&bulk);
   refBasket.push_back(&base1);
   std::cout << "begin for each" << std::endl;
   std::for_each(refBasket.begin(), refBasket.end(), printPrice1);
   std::cout << std::endl;
   return 0;
}
