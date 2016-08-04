/// Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam ut nisi finibus, sodales risus
/// vel, hendrerit risus. Ut sed magna ultricies, congue odio commodo, posuere risus. Cum sociis
/// natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. In feugiat lacus
/// elementum, maximus quam quis, suscipit neque. Curabitur pretium efficitur felis sit amet
/// facilisis. Cras tincidunt pulvinar neque, eu varius sapien gravida quis. Suspendisse potenti.
struct Foo {

    /// Cras tincidunt pulvinar neque, eu varius sapien gravida quis.
    let bar: Int

    /// Suspendisse potenti.
    let baz: Bool

    /// Cras tristique facilisis metus, ac bibendum lorem efficitur at. Vivamus finibus rhoncus
    /// ipsum molestie convallis. Sed ac sem at nisl consectetur ornare. Aenean purus magna,
    /// consequat id sodales sed, rhoncus eu augue. Lorem ipsum dolor sit amet, consectetur
    /// adipiscing elit.
    ///
    /// - parameter qux: Sed dapibus nisi at velit interdum, et.
    init(qux: String) {
        self.bar = 3
        self.baz = false
    }

}