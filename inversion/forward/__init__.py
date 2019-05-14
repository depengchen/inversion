import data_py


class data_model_base:

    def __init__(self):
        super.__init__()
        self.b = data_py.data_model_base()

    def __getitem__(self, idx: str):
        return self.b[idx]

    def __setitem__(self, idx: str, item: data_py.vector_float_t):
        self.b[idx] = item

    def set_item_s(self, idx: str, item):
        self.b[idx] = item

    def __len__(self):
        return self.count

    def get_count(self):
        return self.b.count

    def get_name(self):
        return self.b.name

    def set_name(self, n: str):
        self.b.name = n

    def get_idx(self):
        return self.b['idx']

    def set_idx(self, idx):
        self.b['idx'] = idx

    def load_from_file(self, path: str):
        self.b.load_from_file(path)

    def save_to_file(self, path: str):
        self.b.save_to_file(path)

    count = property(get_count)
    name = property(get_name, set_name)
    idx = property(get_idx, set_idx)


class geoelectric_model(data_model_base):

    def __init__(self):
        self.b = data_py.geoelectric_model()


class isometric_model(data_model_base):

    def __init__(self):
        self.b = data_py.isometric_model()

    def get_height(self):
        return self.b.layer_height

    def set_height(self, height: float):
        self.b.layer_height = height

    height = property(get_height, set_height)


def iso_to_geo(iso: isometric_model) -> geoelectric_model:
    g = geoelectric_model()
    g.b = data_py.iso_to_geo(iso.b)
    return g


class forward_data(data_model_base):

    def __init__(self):
        self.b = data_py.forward_data()

    def generate_time_stamp_by_count(self, exponent_1: float, exponent_2: float, count: int):
        self.b.generate_time_stamp_by_count(exponent_1, exponent_2, count)

    def generate_default_time_stamp(self):
        self.b.generate_default_time_stamp()


class filter_coefficient:

    def __init__(self):
        self.f = data_py.filter_coefficient()

    def load_hkl_coef(self, path: str):
        self.f.load_hkl_coef(path)

    def load_cos_coef(self, path: str):
        self.f.load_cos_coef(path)


class forward_gpu:

    def __init__(self):
        self.fw = data_py.forward_gpu()

    def load_general_params(self, a, i, h):
        self.fw.load_general_params(a, i, h)

    def load_geo_model(self, g: geoelectric_model):
        self.fw.load_geo_model(g.b)

    def load_filter_coef(self, f: filter_coefficient):
        self.fw.load_filter_coef(f.f)

    def load_time_stamp(self, t: forward_data):
        self.fw.load_time_stamp(t.b)

    def init_cuda_device(self):
        self.fw.init_cuda_device()

    def test_cuda_device(self):
        self.fw.test_cuda_device()

    def forward(self):
        self.fw.forward()

    def get_result_late_m(self) -> forward_data:
        ret = forward_data()
        ret.b = self.fw.get_result_late_m()
        return ret

    def get_result_late_e(self) -> forward_data:
        ret = forward_data()
        ret.b = self.fw.get_result_late_e()
        return ret
