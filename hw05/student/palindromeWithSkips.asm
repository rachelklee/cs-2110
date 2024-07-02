private int linProbe(K key) {
        int hash = Math.abs(key.hashCode() % table.length);

        int numProbe = 0;
        int firstDel = -1;

        while ((table[hash] != null) && (numProbe < size)) {
            if (table[hash].getKey().equals(key)) {
                if (!table[hash].isRemoved()) {
                    return hash;
                } else {
                    if (firstDel != -1 && hash < firstDel) {
                        return hash;
                    } else if (firstDel != -1 && firstDel < hash) {
                        return firstDel;
             \       }
                }
                return hash;
            } else if (firstDel == -1 && table[hash].isRemoved()) {
                firstDel = hash;
            }

            if (!table[hash].isRemoved()) {
                ++numProbe;
            }

            hash = Math.abs((++hash) % table.length);
        }

        return hash;
    }