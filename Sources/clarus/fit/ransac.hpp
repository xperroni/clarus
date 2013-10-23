#ifndef RANSAC_HPP
#define RANSAC_HPP

template<class Model> int ransac(
    Model &model,
    const typename Model::Data &data,
    int seed,
    const typename Model::Distance &t,
    int rounds
) {
    Model tentative;
    int best = 0;

    for (int i = 0; i < rounds; i++) {
        typename Model::Data samples(seed, data);
        tentative.fit(samples);

        typename Model::Data consensus;
        for (typename Model::Data::const_iterator j = data.begin(), jn = data.end(); j != jn; ++j) {
            const typename Model::Data::value_type &point = (*j);
            if (tentative.distance(point) < t) {
                consensus.add(point);
            }
        }

        int count = consensus.size();
        if (count > best) {
            best = count;
            model = tentative;
        }
    }

    return best;
}

#endif // RANSAC_HPP
