package com.linkedin.northguard.clients.common;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;


/**
 * A container of header objects. Multiple values per key are allowed. Accessed in insertion order.
 */
public class Headers {
  private static class KVPair {
    public byte[] key;
    public List<Header> value;

    public KVPair(byte[] key, List<Header> value) {
      this.key = key;
      this.value = value;
    }
  }

  private List<KVPair> kvArray = new ArrayList<>();

  public Headers() {}

  /**
   * Append a header to the end of the list of headers for the contained key.
   * @param header A key-value pair.
   * @return this object.
   */
  public Headers add(Header header) {
    List<Header> headerList = searchKey(header.key());
    if (headerList == null) {
      headerList = new ArrayList<>();
      headerList.add(header);
      kvArray.add(new KVPair(header.key(), headerList));
    }
    else {
      headerList.add(header);
    }
    return this;
  }

  /**
   * Get an iterable of headers in insertion order. Calling {@link Iterator#remove()} is modifies this Header.
   * @param key the key. non-null.
   * @return A non-null iterable. Possibly empty.
   */
  public Iterable<Header> get(byte[] key) {
    List<Header> headerList = searchKey(key);
    return headerList == null? Collections.EMPTY_LIST : headerList;
  }

  /**
   * Get the last header for the key in insertion-order
   * @param key the key. non-null
   * @return The last Header if it exists, null otherwise.
   */
  public Header getLast(byte[] key) {
    List<Header> headerList = searchKey(key);
    if (headerList == null || headerList.isEmpty()) {
      return null;
    }
    else {
      return headerList.get(headerList.size() - 1);
    }
  }

  /**
   * Get all the stored Headers. Calling {@link Iterator#remove()} has no effect on this object.
   * @return An iterable of headers.
   */
  public Iterable<Header> getAll() {
    List<Header> all = new ArrayList<>();
    for (KVPair pair: kvArray) {
      for (Header header: pair.value) {
        all.add(header);
      }
    }
    return all;
  }

  /**
   * Remove all the Headers associated with the input key.
   * @param needle the key. non-null
   * @return this object.
   */
  public Headers remove(byte[] needle) {
    for (int i = 0;i < kvArray.size(); i++) {
      if (Arrays.equals(kvArray.get(i).key, needle)) {
        kvArray.remove(i);
        break;
      }
    }
    return this;
  }

  private List<Header> searchKey(byte[] needle) {
    for (KVPair hay: kvArray) {
      if (Arrays.equals(hay.key, needle)) {
        return hay.value;
      }
    }
    return null;
  }
}
