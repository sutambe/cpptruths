#include <iostream>
#include <list>
#include <stdexcept>
#include <stack>
#include <vector>
#include <algorithm>

class Buffer
{
  public:
    typedef unsigned char TYPE;

  public:
    explicit Buffer (size_t size = 0)
      : buf_ (new TYPE [size]),
        size_ (size)      
    {
      memset (buf_, 0, size);
    }

    Buffer (const Buffer &b)
      : buf_ (new TYPE [b.size()]),
        size_ (b.size())
    {
      //std::cout << "Buffer Copy Ctor" << size_ << "\n";
      memcpy (buf_, b.buf_, size_);
    }

    Buffer & operator = (const Buffer &b)
    {
      Buffer temp (b);
      swap (temp);
      return *this;
    }

    TYPE const & operator [] (size_t n) const
    {
      return buf_[n];
    }
    
    TYPE & operator [] (size_t n) 
    {
      return buf_[n];
    }

    TYPE const & get (size_t n) const
    {
      if (n >= size_)
        throw std::range_error ("Out of range");
      return buf_[n];
    }

    TYPE & get (size_t n)
    {
      if (n >= size_)
        throw std::range_error ("Out of range");
      return buf_[n];
    }

    void set (size_t n, const TYPE &val)
    {
      if (n >= size_)
        throw std::range_error ("Out of range");
      else
        buf_[n] = val;
    }
    
    void resize (size_t size)
    {
      Buffer temp (size);
      memcpy (temp.buf_, this->buf_, std::min (this->size_, size));
      swap (temp);
    }

    void swap (Buffer &b) throw ()
    {
      std::swap (this->buf_, b.buf_);
      std::swap (this->size_, b.size_);
    }

    size_t size () const throw ()
    {
      return size_;
    }   

    virtual ~Buffer () throw ()
    {
      delete [] buf_;
    }

  protected:
    TYPE *buf_;
    size_t size_;
};

class UndoBuffer : public Buffer
{
  public:
    class Undo
    {
        friend class UndoBuffer;
        
        size_t position_; 
        UndoBuffer *buffer_;
        Buffer::TYPE old_;
        Buffer::TYPE new_;

        Undo (size_t pos, UndoBuffer *b)
          : position_ (pos),
            buffer_ (b),
            old_ (buffer_->buf_[position_]),
            new_ (0)
        {}

        void execute ()
        {
          buffer_->buf_[position_] = old_;
          Undo u = buffer_->undo_stack_.top();
          buffer_->redo_stack_.push(u);
          buffer_->undo_stack_.pop();
        }
        
        void revert ()
        {
          buffer_->buf_[position_] = new_;
          Undo u = buffer_->redo_stack_.top();
          buffer_->undo_stack_.push (u);
          buffer_->redo_stack_.pop();
        }

        void set_buf (UndoBuffer *ub)
        {
          this->buffer_ = ub;
        }

      public:

        Undo (const Undo &u)
          : position_ (u.position_),
            buffer_ (u.buffer_),
            old_ (u.old_),
            new_ (u.new_)
        {
          //std::cout << "Undo Copy Ctor\n";
        }


        //typedef Undo & Return;
        typedef void Return;
        
        // The following assignment operator is to support the case of
        // ub[i] = ub[j];
        Return operator = (const Undo &u)
        {
          //std::cout << "Undo copy assignment operator\n";
          //return 
            this->operator = (Buffer::TYPE (u)); 
        }

        Return operator = (const Buffer::TYPE & c)
        {
          //std::cout << "Undo::operator = (const Buffer::TYPE & c)\n";
          new_ = c;
          if (old_ != new_)
	  {
	    buffer_->buf_[position_] = new_; 
	    buffer_->undo_stack_.push (*this);
	  }
          // Do not return new_ here. Note: It is return by reference.
	  //return *this;
        }
        
        void swap (Undo & u2)
        {
          std::swap (position_, u2.position_); 
          std::swap (buffer_, u2.buffer_);
          std::swap (old_, u2.old_);
          std::swap (new_, u2.new_);
        }

        Undo & operator ++ ()
        {
          Buffer::TYPE temp = buffer_->buf_[position_] + 1;
          this->operator = (temp); 
          return *this;
        }

        // The post increment operator has to return TYPE or should
        // be disabled altogether. This is because if we return an
        // Undo object by value then the pre-increment and post-increment
        // are indistinguishable to the user. 
        Buffer::TYPE operator ++ (int)
        {
          Buffer::TYPE temp = *this;
          ++(*this);
          return temp;
        }

        // If we use Buffer::TYPE instead of int, compiler throws and error
        // saying that it is ambiguous.
        Return operator += (const int &c)
        {
          Buffer::TYPE temp = *this;
          temp += c;
          this->operator = (temp);
        }  
        
        operator Buffer::TYPE & () const 
        {       
          //std::cout << "Undo::operator Buffer::TYPE &\n";
          return buffer_->buf_[position_];
        }
      
        // non-pod types that have overloaded & operator can't be used with STL
        // unless something like boost::address_of abstraction is used in STL.
        // This program fails in std::uninitilized_copy when it tries to take
        // address.
/*
        Buffer::TYPE * operator & ()
        {
          std::cout << "non-const Undo::operator &\n";
          return &buffer_->buf_[position_];
        }
        
        Buffer::TYPE const * operator & () const
        {
          std::cout << "const Undo::operator &\n";
          return &buffer_->buf_[position_];
        }
*/
    };

    void mod_undo_buffers (std::stack <Undo, std::vector <Undo> > & s)
    {
      std::stack <Undo, std::vector <Undo> > temp;
      while (!s.empty())
      {
        Undo u (s.top ());
        s.pop ();
        u.set_buf (this);
        temp.push (u);
      }
      while (!temp.empty())
      {
        s.push (temp.top());
        temp.pop ();
      }
    }

  public:
    explicit UndoBuffer (size_t size = 0)
      : Buffer (size)
      {}

    UndoBuffer (const UndoBuffer &ub)
      : Buffer (ub),
        undo_stack_ (ub.undo_stack_),
        redo_stack_ (ub.redo_stack_)
    {
      //std::cout << "Copy ctor of UndoBuffer\n";
      mod_undo_buffers (undo_stack_);
      mod_undo_buffers (redo_stack_);
    }

    UndoBuffer & operator = (const UndoBuffer &ub)
    {   
      UndoBuffer temp (ub);
      temp.swap (*this);
      return *this;
    }

    void swap (UndoBuffer &ub) throw ()
    {
      Buffer::swap (ub);
      std::swap (this->undo_stack_, ub.undo_stack_);
      std::swap (this->redo_stack_, ub.redo_stack_);
    }

    using Buffer::operator []; // This brings the const operator in the scope?
    
    TYPE const & operator [] (size_t n) const
    {
      return Buffer::buf_[n];
    }

    Undo operator [] (size_t n)
    {
      return Undo (n, this);
    }

    void redo ()
    {
      if (!redo_stack_.empty())
        this->redo_stack_.top().revert ();
    }
    
    void undo ()
    {
      if (!undo_stack_.empty())
        this->undo_stack_.top().execute ();
    }

    ~UndoBuffer () throw ()
    {}

  private:
    std::stack <Undo, std::vector<Undo> > undo_stack_;
    std::stack <Undo, std::vector<Undo> > redo_stack_;
};

namespace std
{

template <>
void swap <Buffer> (Buffer & b1, Buffer & b2) throw ()
{
  b1.swap (b2);
}

template <>
void swap <UndoBuffer> (UndoBuffer & u1, UndoBuffer & u2) throw ()
{
  u1.swap (u2);
}

template <>
void swap <UndoBuffer::Undo> (UndoBuffer::Undo & u1, UndoBuffer::Undo & u2) throw ()
{
  u1.swap (u2);
}

} // namespace std

int main (void)
{
  try
  {
    std::vector <UndoBuffer> v (10);
    UndoBuffer b (100);
    b[1] = b[2];
    b[0] = 'A';
    UndoBuffer b2 (b);
    b2.undo ();
    std::cout << b[0];
    // The following two compounded assignment expressions although look 
    // quite simple and identical, quite interestingly, their meaning
    // is different because of internal stack. They give two different
    // outputs. This is highly unexpected. So to prevent the programmer
    // from writing such expressions, the assignment operators of Undo
    // should return void.
    //b[0] = b[1] = 'B';
    //b[1] = b[0] = 'B';
    b[0] = 34;
    b[1] += b[0] + b[0];
    b.resize (10);
    b.undo ();
    std::cout << b[0] << b[1] << std::endl;
    b.redo ();
    std::cout << b[0] << b[1] << std::endl;
    b[1]++;
    ++b[1];
    b.undo ();
    std::cout << b[1]++ << !b[1] << (b[1] <= 'E') << std::endl;
    std::cout << "Program is ending\n";
  }
  catch (std::range_error &e)
  {
	  std::cerr << e.what() << std::endl;
  }


  return 0;
}

