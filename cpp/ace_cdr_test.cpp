#include <iostream>
#include "ace/CDR_Stream.h"
#include "ace/Message_Block.h"
#include "ace/Assert.h"

void test_buffer ();
void test_ulonglong ();
int main (void)
{
  test_buffer();
  //test_ulonglong();
  return 0;
}

void test_buffer()
{
  const unsigned int SIZE = ACE_CDR::MAX_ALIGNMENT + 80;
  char buffer [SIZE];
  ACE_Message_Block mblk(buffer, SIZE);
  ACE_OutputCDR cdr (SIZE);
  //ACE_OutputCDR cdr (&mblk);
  for (int i = 0;i < 40; ++i)
  {
    if (! (cdr << ACE_CDR::ULong (i))) std::cout << "error i " << i << std::endl; 
    std::cout << cdr.begin()->total_length() << std::endl;
  }
  cdr.consolidate();

  std::cout << "cdr.good_bit = " << cdr.good_bit() << std::endl;
  std::cout << "length of first message block = " << cdr.length() << std::endl;
  std::cout << "total length of cdr = " << cdr.total_length() << std::endl;
  
}

void test_ulonglong ()
{
  ACE_OutputCDR outcdr (2048);
  ACE_CDR::ULongLong i = 10L;
  ACE_CDR::ULongLong j = 0;

  outcdr.write_ulonglong (i);
  
  const ACE_Message_Block *mblk = outcdr.begin ();  
  //ACE_InputCDR incdr (mblk->rd_ptr(), mblk->size());
  ACE_InputCDR incdr (mblk->rd_ptr(), 15);

  ACE_ASSERT (incdr.read_ulonglong (j));

  ACE_ASSERT (i == j);
}
