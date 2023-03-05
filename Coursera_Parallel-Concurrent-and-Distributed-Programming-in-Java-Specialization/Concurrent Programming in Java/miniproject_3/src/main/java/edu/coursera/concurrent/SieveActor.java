package edu.coursera.concurrent;

import edu.rice.pcdp.Actor;

import static edu.rice.pcdp.PCDP.finish;

import java.util.ArrayList;
import java.util.List;

/**
 * An actor-based implementation of the Sieve of Eratosthenes.
 * <p>
 * TODO Fill in the empty SieveActorActor actor class below and use it from
 * countPrimes to determin the number of primes <= limit.
 */
public final class SieveActor extends Sieve {
    /**
     * {@inheritDoc}
     * <p>
     * TODO Use the SieveActorActor class to calculate the number of primes <=
     * limit in parallel. You might consider how you can model the Sieve of
     * Eratosthenes as a pipeline of actors, each corresponding to a single
     * prime number.
     */
    @Override
    public int countPrimes(final int limit) {
        SieveActorActor sieveActorActor = new SieveActorActor(2);
        finish(() -> {
            for (int i = 3; i <= limit; i += 2) {
                sieveActorActor.send(i);
            }
            sieveActorActor.send(0);
        });
        int cnt = 0;
        SieveActorActor curr = sieveActorActor;
        while (curr != null) {
            cnt += curr.localPrimes.size();
            curr = curr.nextActorActor;
        }

        return cnt;
    }

    /**
     * An actor class that helps implement the Sieve of Eratosthenes in
     * parallel.
     */
    public static final class SieveActorActor extends Actor {
        /**
         * Process a single message sent to this actor.
         * <p>
         * TODO complete this method.
         *
         * @param msg Received message
         */

        static final int MAX_LOC_PRIMES = 1000;

        private List<Integer> localPrimes = new ArrayList<>();
        private SieveActorActor nextActorActor = null;

        SieveActorActor(int i) {
            localPrimes.add(i);
        }

        @Override
        public void process(final Object msg) {
            final int candidate = (Integer) msg;

            if (candidate <= 0) {
                if (nextActorActor != null) {
                    nextActorActor.send(msg);
                }
                return;
            } else {
                if (isLocallyPrime(candidate)) {
                    if (localPrimes.size() < MAX_LOC_PRIMES) {
                        localPrimes.add(candidate);
                    } else if (nextActorActor == null) {
                        nextActorActor = new SieveActorActor(candidate);
                    } else {
                        nextActorActor.send(msg);
                    }
                }
            }
        }

        public boolean isLocallyPrime(final int candidate) {
            for (Integer num : localPrimes) {
                if (candidate % num.intValue() == 0) return false;
            }
            return true;
        }
    }
}
