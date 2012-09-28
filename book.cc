#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>


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

class SalesItem {
public:
    SalesItem():p(0), use(new std::size_t(1)) { }
    //explicit SalesItem(const ItemBook&);
    SalesItem(const ItemBook&);
    SalesItem(const SalesItem &item):p(item.p), use(item.use) {
        std::cout << "call copy construtor of SalesItem" << std::endl;
        ++*use;
        std::cout << *use << std::endl;
    }
    ~SalesItem() {
        std::cout << "call destrutor of SalesItem" << std::endl;
        std::cout << p->getBook() << std::endl;
        std::cout << *use << std::endl;
        decrUse();
        std::cout << "destrutor of SalesItem end" << std::endl;
    }

    SalesItem& operator=(const SalesItem&);
    const ItemBook* operator->() const {
        if (p != NULL)
            return p;
    }
    const ItemBook& operator*() const {
        if (p != NULL)
            return *p;
    }

private:
    ItemBook *p;
    std::size_t *use;
    void decrUse() {
        std::cout << "call decrUse" << std::endl;
        if (--*use == 0) {
            delete p;
            delete use;
        }
        std::cout << "use:\t" << *use << std::endl;
    }
};

SalesItem& SalesItem::operator=(const SalesItem &rhs)
{
    std::cout << "call operator = " << std::endl;
    ++*rhs.use;
    decrUse();
    p = rhs.p;
    use = rhs.use;
    return *this;
}

inline SalesItem::SalesItem(const ItemBook& book):
    p(book.clone()), use(new std::size_t(1))
{
    std::cout << "convert from Itembook to SalesItem" << std::endl;
    std::cout << *use << std::endl;
}

inline bool compare(const SalesItem &lhs, const SalesItem &rhs)
{
    return lhs->getBook() < rhs->getBook();
}

class Basket {
    typedef bool (*comp)(const SalesItem&, const SalesItem&);
public:
    typedef std::multiset<SalesItem, comp> set_type;
    typedef set_type::const_iterator const_iter;
    //construtor 
    Basket():items(compare) { }
    ~Basket() {
        std::cout << "call destrutor of Basket" << std::endl;
    }

    //call copy construtor of Itembook to genarate a template Itembook 
    //then convert Itembook object to SalesItem
    void addItem(const SalesItem &item) {
        std::cout << "call addItem" << std::endl;
        //call copy construtor of SalesItem 
        //items.insert(item);
        std::cout << "call addItem end" << std::endl;
        //call destrutor of SalesItem
    }

    set_type::size_type size(const SalesItem &i) const {
        items.count(i);
    }

    double total() const;
private:
    std::multiset<SalesItem, comp> items;
};

double Basket::total() const 
{
    double sum = 0.0;
    //set_tpye::const_iterator it;
    const_iter it;
    for (it = items.begin(); it != items.end(); it = items.upper_bound(*it))
        sum += (*it)->getPrice(items.count(*it));
}

int main(int argc, const char *argv[])
{
   ItemBook base("----itemBook---", 15);
   //base.printPrice(20);
   //base.show();
   //ItemBook base1("itemBook2", 15);
   //base.printPrice(30);
   //base1.show();
   //BulkBook bulk("bulkBook", 15, 20, 0.1);
   //bulk.printPrice(20);
   //bulk.show();

/*   std::cout << std::endl;*/
   //std::cout << "vector begin" << std::endl;
   //std::vector<ItemBook> basket;
   //basket.push_back(base);
   //basket.push_back(bulk);
   //basket.push_back(base1);
   //std::cout << "begin for each" << std::endl;
   //std::for_each(basket.begin(), basket.end(), printPrice);

   std::cout << std::endl;
   //ItemBook *item = new BulkBook;
   //delete item;

/*   std::cout << "refernce vector begin" << std::endl;*/
   //std::vector<ItemBook*> refBasket;
   //refBasket.push_back(&base);
   //refBasket.push_back(&bulk);
   //refBasket.push_back(&base1);
   //std::cout << "begin for each" << std::endl;
   //std::for_each(refBasket.begin(), refBasket.end(), printPrice1);
   //std::cout << std::endl;

   std::cout << "basket begin" << std::endl;
   Basket basket1;
   basket1.addItem(base);
   std::cout << "basket end" << std::endl;
   //basket1.addItem(base1);
   //basket1.addItem(bulk);
   //std::cout << "basket total:\t" << basket1.total() << std::endl;

   return 0;
}
